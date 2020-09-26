// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include "JsonHandler.generated.h"

UCLASS()
class BIMUNREAL_API AJsonHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJsonHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// 对象反序列化方法
	// ----------------
	static bool Deserialize(const FString & inputJson, TSharedPtr<FJsonObject> * pOutParsed);

	// 对象数组反序列化方法
	// --------------------
	static bool DeserializeList(const FString & inputJson, TArray<TSharedPtr<FJsonValue>> * pOutParsed);
};
