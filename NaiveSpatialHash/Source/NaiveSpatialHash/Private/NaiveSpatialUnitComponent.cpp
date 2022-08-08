// Fill out your copyright notice in the Description page of Project Settings.


#include "NaiveSpatialUnitComponent.h"
#include "NaiveSpatialHashSystem.h"

UNaiveSpatialUnitComponent::UNaiveSpatialUnitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNaiveSpatialUnitComponent::MarkAsDynamicUnit()
{
	bDynamicUnit = true;
}

void UNaiveSpatialUnitComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoRegisterAsUnit)
	{
		if (UNaiveSpatialHashSystem* HashSystem = GetSpatialHashSystem())
		{
			HashSystem->RegisterAsSpatialUnit(GetOwner(), bDynamicUnit);
		}
	}
}

void UNaiveSpatialUnitComponent::BeginDestroy()
{
	Super::BeginDestroy();

	if (UNaiveSpatialHashSystem* HashSystem = GetSpatialHashSystem())
	{
		HashSystem->UnregisterSpatialUnit(GetOwner());
	}
}

class UNaiveSpatialHashSystem* UNaiveSpatialUnitComponent::GetSpatialHashSystem() const
{
	if (AActor* MyOwner = GetOwner())
	{
		UWorld* MyOwnerWorld = MyOwner->GetWorld();
		return MyOwnerWorld ? MyOwnerWorld->GetSubsystem<UNaiveSpatialHashSystem>():nullptr;
	}
	return nullptr;
}

// void UNaiveSpatialUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// }

