// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

#include "../Common/JsonHandler.h"
#include "../Common/OpenCTMHandler.h"
#include "../Common/UrlsHandler.h"
#include "../Model/FElement.h"
#include "../Common/StringHandler.h"

#include "Interfaces/IHttpResponse.h"
#include "Misc/CString.h"

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
		//UE_LOG(LogTemp, Warning, TEXT("all are not null 1543"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("some is nulla"));
	}

	// 地址拼接测试
	// ------------
	AUrlsHandler::InitParameters(FString("7d96928d-5add-45cb-a139-2c787141e50d"), FString("9f49078c-180e-4dc5-b696-5a50a9e09016"), FString("142554"));
	FString allEleurls = AUrlsHandler::GetUrlOfGetAllElements();
	UE_LOG(LogTemp, Error, TEXT("allEleurls == %s"), *allEleurls);

	// 请求接口测试
	// ------------
	TSharedRef<IHttpRequest> httpReuest = FHttpModule::Get().CreateRequest();
	httpReuest->SetVerb(TEXT("GET"));
	httpReuest->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	httpReuest->SetURL(allEleurls);
	httpReuest->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_AllElementsInView);
	httpReuest->ProcessRequest();

	// 请求接口测试2
	// -------------
	FString urlOfCacheBlockCnt = AUrlsHandler::GetUrlOfGetBlockCacheCount();
	TSharedRef<IHttpRequest> httpReuest2 = FHttpModule::Get().CreateRequest();
	httpReuest2->SetVerb(TEXT("GET"));
	httpReuest2->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	httpReuest2->SetURL(urlOfCacheBlockCnt);
	httpReuest2->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_CacheBlockCount);
	httpReuest2->ProcessRequest();

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

			//UE_LOG(LogTemp, Warning, TEXT("-- %s"), *FStringAuthor);
			UE_LOG(LogTemp, Warning, TEXT("-- %s, %d"), *FStringName, sex);
		}
	}
}

void AMyActor::OnRequestComplete_AllElementsInView(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Request.IsValid() || !Response.IsValid())
	{
		return;
	}

	if (bConnectedSuccessfully)
	{
		// 拿到 res，得到接口返回的字符串
		// ------------------------------
		IHttpResponse * res = Response.Get();
		FString ContentJson = Response->GetContentAsString();

		// 反序列化成为数组
		// ----------------
		TArray<TSharedPtr<FJsonValue>> originParsed;
		bool BFlag = AJsonHandler::DeserializeList(ContentJson, &originParsed);
		if (BFlag) 
		{
			// 反序列化成功后，解析为对象
			// --------------------------
			TArray<FElement> eles;

			for (int i = 0; i < originParsed.Num(); i++ )
			{
				// 这里构造 AElement 的对象，但是为什么无法构造？
				// 必须是未继承于 AActor 的类才可以手动构造，并辅助进行数据处理
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

				// 解析 BlockMapping_obj
				// ---------------------
				//ele.BlockMapping_obj.blockMappingItems 
				TSharedPtr<FJsonObject> tempParsed;
				bool tempFlag = AJsonHandler::Deserialize(ele.BlockMapping, &tempParsed);
				if (tempFlag)
				{
					// 这里先硬编码，由于 blockMappingItems 不是个字符串，而是直接的数组
					// 需要 blockMappingItems 换个中间类型，用来接收数组
					// -------------------------------------------------
					ele.BlockMapping_obj.blockMappingItems = tempParsed.Get()->GetArrayField(FString("blockMappingItems"));

					// blockMappingItems 有值了，该构造 blockMappingItems_objlist 了
					// -------------------------------------------------------------
					for (int j = 0; j < ele.BlockMapping_obj.blockMappingItems.Num(); j++)
					{
						auto & item = ele.BlockMapping_obj.blockMappingItems[j];

						// 构造 FBlockMapping_Item
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

			// 打印输出 eles 的个数
			// --------------------
			//UE_LOG(LogTemp, Error, TEXT("7 ---- %d"), (eles[eles.Num() - 1].BlockMapping_obj.blockMappingItems_objlist[0].transform_list.Num()));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("des err!"));
		}
	}
}

void AMyActor::OnRequestComplete_CacheBlockCount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Request.IsValid() || !Response.IsValid())
	{
		return;
	}

	if (bConnectedSuccessfully)
	{
		// 拿到 res，得到接口返回的字符串
		// ------------------------------
		IHttpResponse * res = Response.Get();
		FString ContentJson = Response->GetContentAsString();

		// 将返回的字符串转为数字，作为获得的 block 的个数
		// -----------------------------------------------
		int blockFileCount = FCString::Atoi(*ContentJson);

		// 从1开始，针对每个数字，再调用 https://bimcomposer.probim.cn/api/Model/GetCacheBlock?FileID=1&ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=58080653-18d1-4067-b480-e02c56eb791a&VersionNO=&ViewID=168550
		// -------------------------------------------------------------------------------------------
		for (int i = 1; i <= blockFileCount; i++) 
		{
			UE_LOG(LogTemp, Warning, TEXT("to call GetCacheBlock?FileID= %d"), i);

			// 得到当前的 url 地址
			// -------------------

			// 请求接口测试2
			// -------------
			FString urlOfCacheBlock = AUrlsHandler::GetUrlOfGetCacheBlock(i);
			TSharedRef<IHttpRequest> httpReuestTemp = FHttpModule::Get().CreateRequest();
			httpReuestTemp->SetVerb(TEXT("GET"));
			httpReuestTemp->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
			httpReuestTemp->SetURL(urlOfCacheBlock);
			httpReuestTemp->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_GetCacheBlock);
			httpReuestTemp->ProcessRequest();
		}
	}
}

void AMyActor::OnRequestComplete_GetCacheBlock(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Warning, TEXT("in OnRequestComplete_GetCacheBlock callback"));
	if (!Request.IsValid() || !Response.IsValid())
	{
		return;
	}
	if (bConnectedSuccessfully)
	{
		FString urlReqed = Response.Get()->GetURL();
		UE_LOG(LogTemp, Warning, TEXT("in OnRequestComplete_GetCacheBlock callback, the URL is %s"), *urlReqed);

		// 拿到这个 block 返回的 buffer
		// ----------------------------
		TArray<uint8> resBuffer = Response.Get()->GetContent();
		UE_LOG(LogTemp, Warning, TEXT("buffer length is %d"), resBuffer.Num());

		// 转为字节数组
		// ------------
		BYTE* buffer = reinterpret_cast<BYTE*>(resBuffer.GetData());

		// 调用方法，解析这个字节数组
		// --------------------------
		AMyActor::AnalysisBuffer(buffer, resBuffer.Num());
	}
}

// 解析这个字节数组
// ----------------
void AMyActor::AnalysisBuffer(BYTE * buffer, int size)
{
	int offset = 0;

	// 由于返回的数据可能不止含有一段 |B365|
	// 这里用循环来取多个 blockBuffer
	// ------------------------------
	int looptimes = 0;
	while (offset < size)
	{
		// 输出循环次数
		// ------------
		UE_LOG(LogTemp, Warning, TEXT("looptimes : %d"), looptimes);

		looptimes++;

		// 首先要跳过 |B365|
		// -----------------
		offset += strlen("|B365|");

		// 声明记录 blockId 的长度的变量
		// -----------------------------
		int blockIdLength;

		// 接下来取4个字节，存储到 blockIdLength 中
		// 取完后马上记录偏移位置
		// ----------------------
		memcpy(&blockIdLength, buffer + offset, sizeof(int));
		offset += sizeof(int);

		// 调试输出 blockIdLength 的值
		// ---------------------------
		UE_LOG(LogTemp, Warning, TEXT("blockIdLength = %d"), blockIdLength);

		// 声明 blockIdLength 个字节的 BYTE，用于存储 blockId 的值
		// -------------------------------------------------------
		BYTE * blockIdBuffer = (BYTE*)calloc(blockIdLength, sizeof(BYTE));

		// 拷贝 blockIdLength 个字节到 blockIdBuffer
		// 取完后马上记录偏移位置
		// ----------------------
		memcpy(blockIdBuffer, buffer + offset, blockIdLength);
		TCHAR* blockIdBuffer_utf8 = UTF8_TO_TCHAR(blockIdBuffer);
		offset += blockIdLength;

		// 调试输出 blockIdBuffer_utf8 的值
		// ---------------------------
		UE_LOG(LogTemp, Warning, TEXT("blockIdBuffer_utf8 = %s"), blockIdBuffer_utf8);

		// 接下来应该是 |ID GEO| 这个字符串，拷贝输出验证
		// ----------------------------------------------
		TCHAR ch[9] = { 0 };
		memcpy(ch, buffer + offset, 8);
		TCHAR * ch_uft8 = UTF8_TO_TCHAR(ch);
		UE_LOG(LogTemp, Warning, TEXT("ch_uft8 = %s"), ch_uft8);
		offset += strlen("|ID GEO|");

		// 再读4个字节，拿到contentLength
		// ------------------------------
		int contentLength;
		memcpy(&contentLength, buffer + offset, sizeof(int));
		UE_LOG(LogTemp, Warning, TEXT("contentLength = %d"), contentLength);
		offset += 4;

		// 先读 contentLength 个字节，读完后再偏移contentLength 个字节，进入下一个循环
		// ---------------------------------------------------------------------------
		BYTE * contentBuffer = (BYTE*)calloc(contentLength, sizeof(BYTE));
		memcpy(contentBuffer, buffer + offset, contentLength);
		offset += contentLength;

		// 写完 calloc，马上写 free
		// ------------------------
		if (blockIdBuffer)
		{
			free(blockIdBuffer);
			blockIdBuffer = NULL;
		}
		if (contentBuffer)
		{
			free(contentBuffer);
			contentBuffer = NULL;
		}
	}
}