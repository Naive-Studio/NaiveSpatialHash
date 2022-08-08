// Fill out your copyright notice in the Description page of Project Settings.


#include "NaiveSpatialHashSettings.h"

UNaiveSpatialHashSettings::UNaiveSpatialHashSettings(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/):
 	UDeveloperSettings(ObjectInitializer)
 {
 	CategoryName = TEXT("Plugins");
 	SectionName = TEXT("Spatial Hash");
 }


const FNaiveSpatialVolumeSetting* UNaiveSpatialHashSettings::GetSpatialHashVolumeSetting(class UWorld* InWorld) const
{
	if (InWorld)
	{
		const FString& WorldAssetPath = InWorld->GetFName().ToString();
		for (auto& VolumeSetting : SpatialHashVolumeSettings)
		{
			FString PathName = VolumeSetting.SpatialHashMapName.GetAssetName();
			if (WorldAssetPath.Equals(PathName))
			{
				return &VolumeSetting;
			}
		}
	}

	return nullptr;
}