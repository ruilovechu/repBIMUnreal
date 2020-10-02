// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonHandler.h"
#include "Serialization/JsonSerializer.h"

// Sets default values
AJsonHandler::AJsonHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

FString AJsonHandler::TryGetStringField(TSharedPtr<FJsonValue>& item, FString fieldName, FString defIfFailed)
{
	FString out1;
	if (!item->AsObject()->TryGetStringField(fieldName, out1))
	{
		out1 = defIfFailed;
	}
	return out1;
}

bool AJsonHandler::TryGetBoolField(TSharedPtr<FJsonValue>& item, FString fieldName, bool defIfFailed)
{
	bool out1;
	if (!item->AsObject()->TryGetBoolField(fieldName, out1))
	{
		out1 = defIfFailed;
	}
	return out1;
}

double AJsonHandler::TryGetNumberField(TSharedPtr<FJsonValue>& item, FString fieldName, double defIfFailed)
{
	double out1;
	if (!item->AsObject()->TryGetNumberField(fieldName, out1))
	{
		out1 = defIfFailed;
	}
	return out1;
}

double AJsonHandler::TryGetNumberField_FromObj(TSharedPtr<FJsonObject>& item, FString fieldName, double defIfFailed)
{
	double out1;
	if (!item->TryGetNumberField(fieldName, out1))
	{
		out1 = defIfFailed;
	}
	return out1;
}

FString AJsonHandler::TryGetStringField_FromObj(TSharedPtr<FJsonObject>& item, FString fieldName, FString defIfFailed)
{
	FString out1;
	if (!item->TryGetStringField(fieldName, out1))
	{
		out1 = defIfFailed;
	}
	return out1;
}

bool AJsonHandler::TryGetBoolField_FromObj(TSharedPtr<FJsonObject>& item, FString fieldName, bool defIfFailed)
{
	bool out1;
	if (!item->TryGetBoolField(fieldName, out1))
	{
		out1 = defIfFailed;
	}
	return out1;
}

// 对象反序列化方法
// ----------------
bool AJsonHandler::Deserialize(const FString & inputJson, TSharedPtr<FJsonObject> * pOutParsed)
{
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(inputJson);
	bool bIsOk = FJsonSerializer::Deserialize(JsonReader, *pOutParsed);
	return bIsOk;
}

// 对象数组反序列化方法
// --------------------
bool AJsonHandler::DeserializeList(const FString & inputJson, TArray<TSharedPtr<FJsonValue>> * pOutParsed)
{
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader2 = TJsonReaderFactory<TCHAR>::Create(inputJson);
	bool bIsOk = FJsonSerializer::Deserialize(JsonReader2, *pOutParsed);
	return bIsOk;
}