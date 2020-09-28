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

	// ���Ի�ȡ String �ֶ�
	// --------------------
	static FString TryGetStringField(TSharedPtr<FJsonValue> & item, FString fieldName, FString defIfFailed);

	// ���Ի�ȡ Bool �ֶ�
	// ------------------
	static bool TryGetBoolField(TSharedPtr<FJsonValue>& item, FString fieldName, bool defIfFailed);

	// ���Ի�ȡ double �ֶ�
	// ------------------
	static double TryGetNumberField(TSharedPtr<FJsonValue>& item, FString fieldName, double defIfFailed);

	// ���Ի�ȡ String �ֶ�(������汾)
	// --------------------------------
	static FString TryGetStringField_FromObj(TSharedPtr<FJsonObject> & item, FString fieldName, FString defIfFailed);

	// ���Ի�ȡ Bool �ֶ�(������汾)
	// ------------------------------
	static bool TryGetBoolField_FromObj(TSharedPtr<FJsonObject>& item, FString fieldName, bool defIfFailed);

	// �������л�����
	// ----------------
	static bool Deserialize(const FString & inputJson, TSharedPtr<FJsonObject> * pOutParsed);

	// �������鷴���л�����
	// --------------------
	static bool DeserializeList(const FString & inputJson, TArray<TSharedPtr<FJsonValue>> * pOutParsed);
};
