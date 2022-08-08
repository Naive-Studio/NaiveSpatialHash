// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaiveSpatialHashFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class NAIVESPATIALHASH_API UNaiveSpatialHashFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SpatialHash", meta = (WorldContext = "WorldContextObject"))
	static void FindNearSpatialUnits(const UObject* WorldContextObject, const FVector& OrigionLocation, FVector Extend, TArray<class AActor*>& OutUnit);

	UFUNCTION(BlueprintCallable, Category = "SpatialHash", meta = (WorldContext = "WorldContextObject"))
	static void FindNearSpatialUnitsByClass(const UObject* WorldContextObject, const FVector& OrigionLocation, FVector Extend, class UClass* InClass, TArray<class AActor*>& OutUnit);

	UFUNCTION(BlueprintCallable, Category = "SpatialHash", meta = (WorldContext = "WorldContextObject"))
	static void FindNearSpatialUnitsWithTag(const UObject* WorldContextObject, const FVector& OrigionLocation, FVector Extend, FName Tag, TArray<class AActor*>& OutUnit);
};
