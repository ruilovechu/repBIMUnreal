// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UrlsHandler.generated.h"

// read Config and set each Urls
UCLASS()
class BIMUNREAL_API AUrlsHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUrlsHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	static FString m_ProjectID;
	static FString m_ModelID;
	static FString m_ViewID;

public:	

	// firstly, you should use this function
	// -------------------------------------
	static int InitParameters(FString projectID, FString modelID, FString viewID);

	// get api url like this: https://bimcomposer.probim.cn/api/Model/GetAllElementsInView?ProjectID=7d96928d-5add-45cb-a139-2c787141e50d&ModelID=9f49078c-180e-4dc5-b696-5a50a9e09016&VersionNO=&ViewID=142554
	// you must call InitParameters firstly
	// ------------------------------------
	static FString GetUrlOfGetAllElements();

};
