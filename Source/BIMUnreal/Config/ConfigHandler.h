// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"



#include "ConfigHandler.generated.h"

UCLASS()
class BIMUNREAL_API AConfigHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConfigHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	static FString m_ModelApiUrl;
	static FString m_NewApiUrl;

public:	

	static FString GetModelApiUrl();
	static FString GetNewApiUrl();
};
