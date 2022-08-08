// Fill out your copyright notice in the Description page of Project Settings.


#include "NaiveSpatialHashSystem.h"
#include "NaiveSpatialDataManager.h"
#include "NaiveSpatialHashSettings.h"

UNaiveSpatialHashSystem::UNaiveSpatialHashSystem()
{

}

void UNaiveSpatialHashSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (const UNaiveSpatialHashSettings* SpatialHashSettings = GetDefault<UNaiveSpatialHashSettings>())
	{
		if (const FNaiveSpatialVolumeSetting* VolumeSetting = SpatialHashSettings->GetSpatialHashVolumeSetting(GetWorld()))
		{
			FSoftClassPath SpatialHashDataManagerClassName = SpatialHashSettings->DefaultHashDataClassName;

			TSubclassOf<UNaiveSpatialDataManager> SpatialHashDataManagerClass = SpatialHashDataManagerClassName.IsValid() ? LoadClass<UNaiveSpatialDataManager>(NULL, *SpatialHashDataManagerClassName.ToString(), NULL, LOAD_None, NULL) : nullptr;
			if (SpatialHashDataManagerClass)
			{
				SpatialHashDataManager = NewObject<UNaiveSpatialDataManager>(this, SpatialHashDataManagerClass, TEXT("SpatialHashDataManager"));
				
				FVector Extend = FVector(FMath::Abs(VolumeSetting->SpatialHashVolumeExtend.X), FMath::Abs(VolumeSetting->SpatialHashVolumeExtend.Y), FMath::Abs(VolumeSetting->SpatialHashVolumeExtend.Z));
				SpatialHashDataManager->Initialize(VolumeSetting->SpatialHashVolumeCenter - Extend, VolumeSetting->SpatialHashVolumeCenter + Extend, VolumeSetting->SpatialHashVolumeCellSize);
			}
		}
	}
	
}

void UNaiveSpatialHashSystem::Deinitialize()
{
	if (SpatialHashDataManager)
	{
		SpatialHashDataManager->Deinitialize();
		SpatialHashDataManager = nullptr;
	}
	
	Super::Deinitialize();
}

bool UNaiveSpatialHashSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	UWorld* World = Cast<UWorld>(Outer);
	check(World);

	const UNaiveSpatialHashSettings* SpatialHashSettings = GetDefault<UNaiveSpatialHashSettings>();
	bool bShouldClientHasData = SpatialHashSettings ? SpatialHashSettings->bShouldClientHasSpatialHash : false;

	return bShouldClientHasData ? true : !(World->IsNetMode(NM_Client));
}

ETickableTickType UNaiveSpatialHashSystem::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

bool UNaiveSpatialHashSystem::IsTickable() const
{
	return SpatialHashDataManager ? SpatialHashDataManager->HasDynamicUnit() : false;
}

void UNaiveSpatialHashSystem::Tick(float DeltaTime)
{
	if (SpatialHashDataManager)
	{
		SpatialHashDataManager->Tick(DeltaTime);
	}
}

void UNaiveSpatialHashSystem::RegisterAsSpatialUnit(AActor* InUnit, bool bDynamic)
{
	if (SpatialHashDataManager)
	{
		SpatialHashDataManager->RegisterUnit(InUnit,bDynamic);
	}
}

void UNaiveSpatialHashSystem::UnregisterSpatialUnit(AActor* InUnit)
{
	if (SpatialHashDataManager)
	{
		SpatialHashDataManager->UnregisterUnit(InUnit);
	}
}

class UNaiveSpatialDataManager* UNaiveSpatialHashSystem::GetSpatialHashData() const
{
	return SpatialHashDataManager;
}
