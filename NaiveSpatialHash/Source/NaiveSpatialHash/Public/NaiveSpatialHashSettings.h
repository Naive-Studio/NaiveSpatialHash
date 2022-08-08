// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NaiveSpatialHashSettings.generated.h"


USTRUCT(BlueprintType)
struct FNaiveSpatialVolumeSetting
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "SpatialHash Settings", meta = (AllowedClasses = "World"))
	FSoftObjectPath SpatialHashMapName;

	/* SpatialHash volume min */
	UPROPERTY(EditAnywhere, Category = "SpatialHash Settings", meta = (DisplayName = "SpatialHash Volume Center"))
	FVector SpatialHashVolumeCenter;

	/* SpatialHash volume max */
	UPROPERTY(EditAnywhere, Category = "SpatialHash Settings", meta = (DisplayName = "SpatialHash Volume Extend"))
	FVector SpatialHashVolumeExtend;

	/* SpatialHash volume resolution */
	UPROPERTY(EditAnywhere, Category = "SpatialHash Settings", meta = (DisplayName = "SpatialHash Volume CellSize"))
	FVector SpatialHashVolumeCellSize;
};

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Spatial Hash"))
class NAIVESPATIALHASH_API UNaiveSpatialHashSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UNaiveSpatialHashSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(config, EditAnywhere, Category = "SpatialHash Settings", meta = (MetaClass = "UNaiveSpatialDataManager", DisplayName = "Default Spatial Hash Data Class"))
	FSoftClassPath DefaultHashDataClassName;

	UPROPERTY(config, EditAnywhere, Category = "SpatialHash Settings", meta = (MetaClass = "UNaiveSpatialDataManager", DisplayName = "Should Client Has Spatial Hash Data"))
	bool bShouldClientHasSpatialHash = false;

	UPROPERTY(config, EditAnywhere, Category = "SpatialHash Settings")
	TArray<FNaiveSpatialVolumeSetting> SpatialHashVolumeSettings;

	const FNaiveSpatialVolumeSetting* GetSpatialHashVolumeSetting(class UWorld* InWorld) const;
};
