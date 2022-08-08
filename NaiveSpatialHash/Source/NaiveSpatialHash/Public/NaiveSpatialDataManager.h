// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NaiveSpatialDataManager.generated.h"

class UNaiveSpatialUnitComponent;
/**
 * 
 */
UCLASS(Abstract)
class NAIVESPATIALHASH_API UNaiveSpatialDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	UNaiveSpatialDataManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Initialize(FVector VolumeMin,FVector VolumeMax, FVector CellSize) {}

	virtual void Deinitialize() {}

	virtual void RegisterUnit(AActor* InUnit,bool bDynamic) {}

	virtual void UnregisterUnit(AActor* InUnit) {}

	virtual void Tick(float DeltaSeconds) {}

	virtual bool HasDynamicUnit() const { return false; }

	virtual void FindNearUnits(const FVector& OrigionLocation, const FVector& Extend, TArray<AActor*>& OutUints) {}
};
