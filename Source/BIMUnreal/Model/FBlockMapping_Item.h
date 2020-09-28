// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BIMUNREAL_API FBlockMapping_Item
{
public:
	FString blockId;
	FString	materialId;

	FString transform;
	TArray<double> transform_list;
	
	bool rpc;
};
