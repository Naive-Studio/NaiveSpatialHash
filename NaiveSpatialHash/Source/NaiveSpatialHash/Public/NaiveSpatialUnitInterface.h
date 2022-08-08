// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NaiveSpatialUnitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNaiveSpatialUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NAIVESPATIALHASH_API INaiveSpatialUnitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual class UNaiveSpatialUnitComponent* GetSpatialUnitComponent() const { return nullptr; }
};
