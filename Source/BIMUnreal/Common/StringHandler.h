// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StringHandler.generated.h"

UCLASS()
class BIMUNREAL_API AStringHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStringHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	static TArray<double> SplitToDoubleArray(const FString& JsonInput, const TCHAR * Splitter);
};
