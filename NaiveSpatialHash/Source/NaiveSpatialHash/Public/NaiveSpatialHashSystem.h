// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NaiveSpatialHashSystem.generated.h"

class AActor;
/**
 * 
 */
UCLASS()
class NAIVESPATIALHASH_API UNaiveSpatialHashSystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UNaiveSpatialHashSystem();

	UFUNCTION(BlueprintCallable, Category = "Spatial Hash")
	void RegisterAsSpatialUnit(AActor* InUnit, bool bDynamic);

	UFUNCTION(BlueprintCallable, Category = "Spatial Hash")
	void UnregisterSpatialUnit(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Spatial Hash")
	class UNaiveSpatialDataManager* GetSpatialHashData() const;

protected:
	//~USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End of USubsystem interface

	//~FTickableGameObject interface
	ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UNaiveSpatialHashSystem, STATGROUP_Tickables); }
	//~End of FTickableGameObject interface

protected:
	UPROPERTY(Transient)
	class UNaiveSpatialDataManager* SpatialHashDataManager;

};
