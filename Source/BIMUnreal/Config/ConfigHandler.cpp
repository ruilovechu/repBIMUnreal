// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigHandler.h"

FString AConfigHandler::m_ModelApiUrl = "https://bimcomposer.probim.cn";
FString AConfigHandler::m_NewApiUrl = "https://www.probim.cn:8080";

// Sets default values
AConfigHandler::AConfigHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConfigHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

FString AConfigHandler::GetModelApiUrl()
{
	return m_ModelApiUrl;
}

FString AConfigHandler::GetNewApiUrl()
{
	return m_NewApiUrl;
}
