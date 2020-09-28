// Fill out your copyright notice in the Description page of Project Settings.


#include "StringHandler.h"

// Sets default values
AStringHandler::AStringHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStringHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<double> AStringHandler::SplitToDoubleArray(const FString & JsonInput, const TCHAR * Splitter)
{
	TArray<FString> jsonInputArray;
	TArray<double> transformArray;

	JsonInput.ParseIntoArray(jsonInputArray, Splitter, true);

	for (size_t i = 0; i < jsonInputArray.Num(); i++)
	{
		transformArray.Emplace(FCString::Atof(*jsonInputArray[i]));
	}

	return transformArray;
}
