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

	// 尝试获取 String 字段
	// --------------------
	static FString TryGetStringField(TSharedPtr<FJsonValue> & item, FString fieldName, FString defIfFailed);

	// 尝试获取 Bool 字段
	// ------------------
	static bool TryGetBoolField(TSharedPtr<FJsonValue>& item, FString fieldName, bool defIfFailed);

	// 尝试获取 double 字段
	// ------------------
	static double TryGetNumberField(TSharedPtr<FJsonValue>& item, FString fieldName, double defIfFailed);

	// 尝试获取 String 字段(单对象版本)
	// --------------------------------
	static FString TryGetStringField_FromObj(TSharedPtr<FJsonObject> & item, FString fieldName, FString defIfFailed);

	// 尝试获取 Bool 字段(单对象版本)
	// ------------------------------
	static bool TryGetBoolField_FromObj(TSharedPtr<FJsonObject>& item, FString fieldName, bool defIfFailed);

	// 对象反序列化方法
	// ----------------
	static bool Deserialize(const FString & inputJson, TSharedPtr<FJsonObject> * pOutParsed);

	// 对象数组反序列化方法
	// --------------------
	static bool DeserializeList(const FString & inputJson, TArray<TSharedPtr<FJsonValue>> * pOutParsed);
};
