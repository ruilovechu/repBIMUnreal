// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MyLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class BIMUNREAL_API AMyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	// a test int value
	// ----------------
	UPROPERTY(EditAnywhere, Category="Test", BlueprintReadwrite)
	int m_TestVal;
};
