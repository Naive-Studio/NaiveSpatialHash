// Fill out your copyright notice in the Description page of Project Settings.


#include "NaiveSpatialGrid.h"

DEFINE_STAT(STAT_SpatialHash_SimpleGridMemory);
DEFINE_STAT(STAT_SpatialHash_UpdateDynamicUnits);
DEFINE_STAT(STAT_SpatialHash_QueryNearbyUnits);

void UNaiveSpatialGrid::Initialize(FVector VolumeMin, FVector VolumeMax, FVector CellSize)
{
	SimpleGridCellSizeX = CellSize.X;
	SimpleGridCellSizeY = CellSize.Y;
	VolumeMinX = VolumeMin.X;
	VolumeMinY = VolumeMin.Y;

	VolumeMaxX = VolumeMax.X;
	VolumeMaxY = VolumeMax.Y;

	CellNumPerRow = FMath::CeilToInt((VolumeMax.X - VolumeMin.X) / CellSize.X) + 1;
	CellNumPerColumn = FMath::CeilToInt((VolumeMax.Y - VolumeMin.Y) / CellSize.Y) + 1;
	CellNumber = CellNumPerColumn * CellNumPerRow;

	GridHashMap.Empty();
	DynamicUnits.Empty();
}

void UNaiveSpatialGrid::Deinitialize()
{
	GridHashMap.Empty();
	DynamicUnits.Empty();
}

void UNaiveSpatialGrid::RegisterUnit(AActor* InUnit, bool bDynamic)
{
	check(InUnit);

	int32 HashIndex = CalculateHashIndex(InUnit->GetActorLocation());
	if (HashIndex != INDEX_NONE)
	{
		FNaiveGridCell& Cell = GridHashMap.FindOrAdd(HashIndex);
		Cell.HashUnitBucket.Add(InUnit);

		if (bDynamic)
		{
			DynamicUnits.Add(InUnit, HashIndex);
		}

		SET_MEMORY_STAT(STAT_SpatialHash_SimpleGridMemory, GridHashMap.GetAllocatedSize()+ DynamicUnits.GetAllocatedSize());
	}
}

void UNaiveSpatialGrid::UnregisterUnit(AActor* InUnit)
{
	check(InUnit);

	int32 HashIndex = CalculateHashIndex(InUnit->GetActorLocation());

	if (int32* HashIndexPtr = DynamicUnits.Find(InUnit))
	{
		HashIndex = DynamicUnits[InUnit];
		DirtyDynamicUnits.Add(InUnit);
	}

	RemoveUnitFrommHashMap(InUnit, HashIndex);

	SET_MEMORY_STAT(STAT_SpatialHash_SimpleGridMemory, GridHashMap.GetAllocatedSize() + DynamicUnits.GetAllocatedSize());
}

void UNaiveSpatialGrid::Tick(float DeltaSeconds)
{
	SCOPE_CYCLE_COUNTER(STAT_SpatialHash_UpdateDynamicUnits);
	//Removing the dirty dynamic units
	for (AActor* Unit : DirtyDynamicUnits)
	{
		DynamicUnits.Remove(Unit);
	}
	DirtyDynamicUnits.Empty();

	for (auto& Unit : DynamicUnits)
	{
		RefreshDynamicUnit(Unit.Key, Unit.Value);
	}
}

bool UNaiveSpatialGrid::HasDynamicUnit() const
{
	return DynamicUnits.Num() > 0;
}

void UNaiveSpatialGrid::FindNearUnits(const FVector& OrigionLocation, const FVector& Extend, TArray<AActor*>& OutUints)
{
	SCOPE_CYCLE_COUNTER(STAT_SpatialHash_QueryNearbyUnits);

	int32 OrigionHashIndex = CalculateHashIndex(OrigionLocation);

	if (OrigionHashIndex != INDEX_NONE)
	{
		int32 MinXIndex = FMath::Max(FMath::FloorToInt((OrigionLocation.X - FMath::Abs(Extend.X) - VolumeMinX) / SimpleGridCellSizeX), 0);
		int32 MaxXIndex = FMath::Min(FMath::FloorToInt((OrigionLocation.X + FMath::Abs(Extend.X) - VolumeMinX) / SimpleGridCellSizeX), CellNumPerRow - 1);

		int32 MinYIndex = FMath::Max(FMath::FloorToInt((OrigionLocation.Y - FMath::Abs(Extend.Y) - VolumeMinY) / SimpleGridCellSizeY), 0);
		int32 MaxYIndex = FMath::Min(FMath::FloorToInt((OrigionLocation.Y + FMath::Abs(Extend.Y) - VolumeMinY) / SimpleGridCellSizeY), CellNumPerColumn - 1);

		if (MinXIndex >= CellNumPerRow || MaxXIndex < 0 || MinYIndex >= CellNumPerColumn || MaxYIndex < 0)
		{
			return;
		}

		for (int32 IndexX = MinXIndex; IndexX <= MaxXIndex; IndexX++)
		{
			for (int32 IndexY = MinYIndex; IndexY <= MaxYIndex; IndexY++)
			{
				int32 HashIndex = IndexX + IndexY * CellNumPerRow;
				if (FNaiveGridCell* Cell = GridHashMap.Find(HashIndex))
				{
					OutUints.Append(Cell->HashUnitBucket.Array());
				}
			}
		}
	}
}

int32 UNaiveSpatialGrid::CalculateHashIndex(const FVector& InLocation) const
{
	int32 IndexX = FMath::FloorToInt((InLocation.X - VolumeMinX) / SimpleGridCellSizeX);
	int32 IndexY = FMath::FloorToInt((InLocation.Y - VolumeMinY) / SimpleGridCellSizeY);

	if (IndexX < 0 || IndexX >= CellNumPerRow || IndexY < 0 || IndexY >= CellNumPerColumn)
	{
		return INDEX_NONE;
	}

	return IndexX + IndexY * CellNumPerRow;
}

void UNaiveSpatialGrid::RefreshDynamicUnit(AActor* InUnit, int32 OldHashIndex)
{
	if (IsValid(InUnit))
	{
		int32 NewHashIndex = CalculateHashIndex(InUnit->GetActorLocation());

		if (NewHashIndex != OldHashIndex)
		{
			RemoveUnitFrommHashMap(InUnit, OldHashIndex);

			if (NewHashIndex != INDEX_NONE)
			{
				GridHashMap.FindOrAdd(NewHashIndex).HashUnitBucket.Add(InUnit);
				DynamicUnits[InUnit] = NewHashIndex;
			}
		}
	}
}

void UNaiveSpatialGrid::RemoveUnitFrommHashMap(AActor* InUnit, int32 HashIndex)
{
	if (FNaiveGridCell* OldCell = GridHashMap.Find(HashIndex))
	{
		OldCell->HashUnitBucket.Remove(InUnit);

		if (OldCell->HashUnitBucket.Num() == 0)
		{
			GridHashMap.Remove(HashIndex);
		}
	}
}

FVector UNaiveSpatialGrid::GetCellCenterLocation(int32 CellIndex)
{
	int32 CellXIndex = CellIndex % CellNumPerRow;
	int32 CellYIndex = CellIndex / CellNumPerRow;

	return FVector(VolumeMinX + CellXIndex * SimpleGridCellSizeX + 0.5f * SimpleGridCellSizeX, VolumeMinY + CellYIndex * SimpleGridCellSizeY + 0.5f * SimpleGridCellSizeY, 0.0f);
}
