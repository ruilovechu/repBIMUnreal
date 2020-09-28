// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BIMUNREAL_API FElement
{
public:
	FString ElementID;
	FString	Name;
	FString	TypeID;
	FString TypeName;
	FString FamilyID;
	FString FamilyName;
	FString CategoryID;
	FString CategoryName;
	FString Properties;
	FString LevelID;
	FString LinkID;
	FString LinkLevelID;
	FString BlockMapping;
	FString Override;
	FString SourceElementID;
	FString MergeParent;
	bool IsMerge;
	bool Is2D;
	FString Batch;
	FString ViewOverride;
};
