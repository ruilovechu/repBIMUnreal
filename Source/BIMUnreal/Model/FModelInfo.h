// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "../Model/FViewInfo.h"

/**
 * 
 */
class BIMUNREAL_API FModelInfo
{
public:
	FString ProjectID;
	FString	Name;
	FString ID;

	TArray<TSharedPtr<FJsonValue>> Views;
	TArray<FViewInfo> Views_objlist;
};
