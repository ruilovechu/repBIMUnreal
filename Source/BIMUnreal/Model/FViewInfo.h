// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Model/FViewOverrideInfo.h"

/**
 * 
 */
class BIMUNREAL_API FViewInfo
{
public:
	FString ID;
	FString	Name;
	FString ModelID;
	FString Override;
	FViewOverrideInfo Override_obj;

	bool IsDefault;
};
