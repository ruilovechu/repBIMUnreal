// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "../Common/JsonHandler.h"
#include "../Common/OpenCTMHandler.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// openCTM dll 引入测试
	// --------------------
	if (AOpenCTMHandler::m_CtmAddUVMap && AOpenCTMHandler::m_CtmCompressionLevel)
	{
		UE_LOG(LogTemp, Warning, TEXT("all are not null 1543"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("some is nulla"));
	}

	// json 操作测试
	// -------------
	FString server_data;
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&server_data);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("name", TEXT("xueys"));
	JsonWriter->WriteValue("password", TEXT("000000"));
	JsonWriter->WriteValue("name1", ("xueys"));
	JsonWriter->WriteValue("password1", ("000000"));
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *server_data);

	TSharedPtr<FJsonObject> JsonObject;
	bool bIsOk = AJsonHandler::Deserialize(server_data, &JsonObject);
	if (bIsOk)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsOK"));

		FString dataname = JsonObject->GetStringField("name");
		FString datapwd = JsonObject->GetStringField("password");
		UE_LOG(LogTemp, Warning, TEXT("++ %s"), *dataname);
		UE_LOG(LogTemp, Warning, TEXT("++ %s"), *datapwd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IsNotOK"));
	}

	// 数组反序列化
	// ------------
	FString JsonValue = "[{\"author\":\"4446545646544\", \"name\":\"a1123\", \"sex\":false},{\"author\":\"jack\", \"name\":\"a1123456\", \"sex\":true}]";
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	bool BFlag = AJsonHandler::DeserializeList(JsonValue, &JsonParsed);
	if (BFlag)
	{
		int ArrayNumber = JsonParsed.Num();
		for (int i = 0; i < ArrayNumber; i++)
		{
			FString FStringAuthor = JsonParsed[i]->AsObject()->GetStringField("author");
			FString FStringName = JsonParsed[i]->AsObject()->GetStringField("name");
			bool sex = JsonParsed[i]->AsObject()->GetBoolField("sex");

			UE_LOG(LogTemp, Warning, TEXT("-- %s"), *FStringAuthor);
			UE_LOG(LogTemp, Warning, TEXT("-- %s, %d"), *FStringName, sex);
		}
	}




}

//// Called every frame
//void AMyActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

