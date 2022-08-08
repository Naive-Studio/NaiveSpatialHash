// Fill out your copyright notice in the Description page of Project Settings.


#include "NaiveSpatialHashFunctionLibrary.h"
#include "NaiveSpatialHashSystem.h"
#include "NaiveSpatialDataManager.h"

void UNaiveSpatialHashFunctionLibrary::FindNearSpatialUnits(const UObject* WorldContextObject, const FVector& OrigionLocation, FVector Extend, TArray<class AActor*>& OutUnit)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	UNaiveSpatialHashSystem* SpatialHashSystem = World ? World->GetSubsystem<UNaiveSpatialHashSystem>() : nullptr;
	UNaiveSpatialDataManager* SpatialHashData = SpatialHashSystem ? SpatialHashSystem->GetSpatialHashData() : nullptr;
	if (SpatialHashData)
	{
		SpatialHashData->FindNearUnits(OrigionLocation, Extend, OutUnit);
	}
}

void UNaiveSpatialHashFunctionLibrary::FindNearSpatialUnitsByClass(const UObject* WorldContextObject, const FVector& OrigionLocation, FVector Extend, class UClass* InClass, TArray<class AActor*>& OutUnit)
 {
	TArray<AActor*> NearUnits;
	FindNearSpatialUnits(WorldContextObject, OrigionLocation, Extend, NearUnits);
	for (AActor* Unit : NearUnits)
	{
		if (Unit && Unit->IsA(InClass))
		{
			OutUnit.Add(Unit);
		}
	}
 }

void UNaiveSpatialHashFunctionLibrary::FindNearSpatialUnitsWithTag(const UObject* WorldContextObject, const FVector& OrigionLocation, FVector Extend, FName Tag, TArray<class AActor*>& OutUnit)
{
	TArray<AActor*> NearUnits;
	FindNearSpatialUnits(WorldContextObject, OrigionLocation, Extend, NearUnits);
	for (AActor* Unit : NearUnits)
	{
		if (Unit && Unit->ActorHasTag(Tag))
		{
			OutUnit.Add(Unit);
		}
	}
}
