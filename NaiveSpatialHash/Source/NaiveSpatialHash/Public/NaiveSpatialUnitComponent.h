// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaiveSpatialUnitComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAIVESPATIALHASH_API UNaiveSpatialUnitComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNaiveSpatialUnitComponent();

	UFUNCTION(BlueprintCallable, Category = "SpatialHash")
	void MarkAsDynamicUnit();

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	class UNaiveSpatialHashSystem* GetSpatialHashSystem() const;

// public:	
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** */
	UPROPERTY(EditAnywhere, Category = "SpatialHash")
	bool bDynamicUnit = false;

	UPROPERTY(EditAnywhere, Category = "SpatialHash")
	bool bAutoRegisterAsUnit = true;
};
