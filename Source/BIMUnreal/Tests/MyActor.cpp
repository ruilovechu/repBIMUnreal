// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

#include "../Common/JsonHandler.h"
#include "../Common/OpenCTMHandler.h"
#include "../Common/UrlsHandler.h"
#include "../Model/FElement.h"
#include "../Common/StringHandler.h"

#include "Interfaces/IHttpResponse.h"

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

	// openCTM dll �������
	// --------------------
	if (AOpenCTMHandler::m_CtmAddUVMap && AOpenCTMHandler::m_CtmCompressionLevel)
	{
		//UE_LOG(LogTemp, Warning, TEXT("all are not null 1543"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("some is nulla"));
	}

	// ��ַƴ�Ӳ���
	// ------------
	AUrlsHandler::InitParameters(FString("7d96928d-5add-45cb-a139-2c787141e50d"), FString("9f49078c-180e-4dc5-b696-5a50a9e09016"), FString("142554"));
	FString allEleurls = AUrlsHandler::GetUrlOfGetAllElements();
	UE_LOG(LogTemp, Error, TEXT("allEleurls == %s"), *allEleurls);

	// ����ӿڲ���
	// ------------
	TSharedRef<IHttpRequest> httpReuest = FHttpModule::Get().CreateRequest();
	httpReuest->SetVerb(TEXT("GET"));
	httpReuest->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	httpReuest->SetURL(allEleurls);
	httpReuest->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete);
	httpReuest->ProcessRequest();

	// json ��������
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

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *server_data);

	TSharedPtr<FJsonObject> JsonObject;
	bool bIsOk = AJsonHandler::Deserialize(server_data, &JsonObject);
	if (bIsOk)
	{
		//UE_LOG(LogTemp, Warning, TEXT("IsOK"));

		FString dataname = JsonObject->GetStringField("name");
		FString datapwd = JsonObject->GetStringField("password");
		//UE_LOG(LogTemp, Warning, TEXT("++ %s"), *dataname);
		//UE_LOG(LogTemp, Warning, TEXT("++ %s"), *datapwd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IsNotOK"));
	}

	// ���鷴���л�
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

			//UE_LOG(LogTemp, Warning, TEXT("-- %s"), *FStringAuthor);
			UE_LOG(LogTemp, Warning, TEXT("-- %s, %d"), *FStringName, sex);
		}
	}
}

void AMyActor::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Request.IsValid() || !Response.IsValid())
	{
		return;
	}

	if (bConnectedSuccessfully)
	{
		// �õ� res���õ��ӿڷ��ص��ַ���
		// ------------------------------
		IHttpResponse * res = Response.Get();
		FString ContentJson = Response->GetContentAsString();

		// �����л���Ϊ����
		// ----------------
		TArray<TSharedPtr<FJsonValue>> originParsed;
		bool BFlag = AJsonHandler::DeserializeList(ContentJson, &originParsed);
		if (BFlag) 
		{
			// �����л��ɹ��󣬽���Ϊ����
			// --------------------------
			TArray<FElement> eles;

			for (int i = 0; i < originParsed.Num(); i++ )
			{
				// ���ﹹ�� AElement �Ķ��󣬵���Ϊʲô�޷����죿
				// ������δ�̳��� AActor ����ſ����ֶ����죬�������������ݴ���
				// ------------------------------------------------------------
				FElement ele;
				ele.ElementID = AJsonHandler::TryGetStringField(originParsed[i], FString("ElementID"), FString(""));
				ele.Name = AJsonHandler::TryGetStringField(originParsed[i], FString("Name"), FString(""));
				ele.TypeID = AJsonHandler::TryGetStringField(originParsed[i], FString("TypeID"), FString(""));
				ele.TypeName = AJsonHandler::TryGetStringField(originParsed[i], FString("TypeName"), FString(""));
				ele.FamilyID = AJsonHandler::TryGetStringField(originParsed[i], FString("FamilyID"), FString(""));
				ele.FamilyName = AJsonHandler::TryGetStringField(originParsed[i], FString("FamilyName"), FString(""));
				ele.CategoryID = AJsonHandler::TryGetStringField(originParsed[i], FString("CategoryID"), FString(""));
				ele.CategoryName = AJsonHandler::TryGetStringField(originParsed[i], FString("CategoryName"), FString(""));
				ele.Properties = AJsonHandler::TryGetStringField(originParsed[i], FString("Properties"), FString(""));
				ele.LevelID = AJsonHandler::TryGetStringField(originParsed[i], FString("LevelID"), FString(""));
				ele.LinkID = AJsonHandler::TryGetStringField(originParsed[i], FString("LinkID"), FString(""));
				ele.LinkLevelID = AJsonHandler::TryGetStringField(originParsed[i], FString("LinkLevelID"), FString(""));
				ele.BlockMapping = AJsonHandler::TryGetStringField(originParsed[i], FString("BlockMapping"), FString(""));
				ele.Override = AJsonHandler::TryGetStringField(originParsed[i], FString("Override"), FString(""));
				ele.SourceElementID = AJsonHandler::TryGetStringField(originParsed[i], FString("SourceElementID"), FString(""));
				ele.MergeParent = AJsonHandler::TryGetStringField(originParsed[i], FString("MergeParent"), FString(""));
				ele.IsMerge = AJsonHandler::TryGetBoolField(originParsed[i], FString("IsMerge"), false);
				ele.Is2D = AJsonHandler::TryGetBoolField(originParsed[i], FString("Is2D"), false);
				ele.Batch = AJsonHandler::TryGetStringField(originParsed[i], FString("Batch"), FString(""));
				ele.ViewOverride = AJsonHandler::TryGetStringField(originParsed[i], FString("ViewOverride"), FString(""));

				// ���� BlockMapping_obj
				// ---------------------
				//ele.BlockMapping_obj.blockMappingItems 
				TSharedPtr<FJsonObject> tempParsed;
				bool tempFlag = AJsonHandler::Deserialize(ele.BlockMapping, &tempParsed);
				if (tempFlag)
				{
					// ������Ӳ���룬���� blockMappingItems ���Ǹ��ַ���������ֱ�ӵ�����
					// ��Ҫ blockMappingItems �����м����ͣ�������������
					// -------------------------------------------------
					ele.BlockMapping_obj.blockMappingItems = tempParsed.Get()->GetArrayField(FString("blockMappingItems"));

					// blockMappingItems ��ֵ�ˣ��ù��� blockMappingItems_objlist ��
					// -------------------------------------------------------------
					for (int j = 0; j < ele.BlockMapping_obj.blockMappingItems.Num(); j++)
					{
						auto & item = ele.BlockMapping_obj.blockMappingItems[j];

						// ���� FBlockMapping_Item
						// -----------------------
						FBlockMapping_Item blockMappingItem;
						blockMappingItem.blockId = AJsonHandler::TryGetStringField(item, FString("blockId"), FString(""));
						blockMappingItem.materialId = AJsonHandler::TryGetStringField(item, FString("materialId"), FString(""));
						blockMappingItem.rpc = AJsonHandler::TryGetBoolField(item, FString("rpc"), false);
						blockMappingItem.transform = AJsonHandler::TryGetStringField(item, FString("transform"), FString(""));
						blockMappingItem.transform_list = AStringHandler::SplitToDoubleArray(blockMappingItem.transform, TEXT(","));

						// blockMappingItems => blockMappingItems_objlist
						// ----------------------------------------------
						ele.BlockMapping_obj.blockMappingItems_objlist.Add(blockMappingItem);
					}
				}

				eles.Add(ele);
			}

			// ��ӡ��� eles �ĸ���
			// --------------------
			UE_LOG(LogTemp, Error, TEXT("7 ---- %d"), (eles[eles.Num() - 1].BlockMapping_obj.blockMappingItems_objlist[0].transform_list.Num()));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("des err!"));
		}
	}
}

//// Called every frame
//void AMyActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

