// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NaiveSpatialDataManager.h"
#include "Stats/Stats2.h"
#include "NaiveSpatialGrid.generated.h"

class AActor;

DECLARE_STATS_GROUP(TEXT("SpatialHash"), STATGROUP_SpatialHash, STATCAT_Advanced);
DECLARE_MEMORY_STAT_EXTERN(TEXT("Spatial Hash Simple Grid Memory"), STAT_SpatialHash_SimpleGridMemory, STATGROUP_SpatialHash, );
DECLARE_CYCLE_STAT_EXTERN(TEXT("Update dynamic spatial hash units"), STAT_SpatialHash_UpdateDynamicUnits, STATGROUP_SpatialHash, );
DECLARE_CYCLE_STAT_EXTERN(TEXT("Query Nearby units"), STAT_SpatialHash_QueryNearbyUnits, STATGROUP_SpatialHash, );

USTRUCT(BlueprintType)
struct FNaiveGridCell
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSet<AActor*> HashUnitBucket;
};


/**
 * 2D Grid
 */
UCLASS(BlueprintType,Blueprintable)
class NAIVESPATIALHASH_API UNaiveSpatialGrid : public UNaiveSpatialDataManager
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FVector VolumeMin, FVector VolumeMax, FVector CellSize) override;

	virtual void Deinitialize() override;

	virtual void RegisterUnit(AActor* InUnit, bool bDynamic) override;

	virtual void UnregisterUnit(AActor* InUnit) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual bool HasDynamicUnit() const override;

	virtual void FindNearUnits(const FVector& OrigionLocation, const FVector& Extend, TArray<AActor*>& OutUints) override;

protected:
	int32 CalculateHashIndex(const FVector& InLocation) const;

	void RefreshDynamicUnit(AActor* InUnit , int32 OldHashIndex);

	void RemoveUnitFrommHashMap(AActor* InUnit, int32 HashIndex);

	FVector GetCellCenterLocation(int32 CellIndex);

protected:
	UPROPERTY(EditAnywhere, Category = "Simple Hash Grid")
	float SimpleGridCellSizeX = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Simple Hash Grid")
	float SimpleGridCellSizeY = 1000.f;

	int32 CellNumPerRow = 0;
	int32 CellNumPerColumn = 0;

	int32 CellNumber = 0;

	float VolumeMinX = 0.0f;
	float VolumeMinY = 0.0f;

	float VolumeMaxX = 0.0f;
	float VolumeMaxY = 0.0f;

	UPROPERTY(Transient)
	TMap<int32, FNaiveGridCell> GridHashMap;

	UPROPERTY(Transient)
	TMap<AActor*, int32> DynamicUnits;

	UPROPERTY(Transient)
	TArray<AActor*> DirtyDynamicUnits;
};
