// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

#include "../Common/JsonHandler.h"
#include "../Common/OpenCTMHandler.h"
#include "../Common/UrlsHandler.h"
#include "../Model/FElement.h"
#include "../Common/StringHandler.h"

#include "Interfaces/IHttpResponse.h"
#include "Misc/CString.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "UObject/ConstructorHelpers.h"

#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapperModule.h"

#include "Engine/Texture2DDynamic.h"
#include "Engine/Engine.h"

#define VERTEX_3		3
#define INDICATE_3		3
#define VERTEX_UV_2		2

// ·��תΪ UTexture2D
// -------------------
void AMyActor::setTextureFromLoadImg(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful)
{
	if (!bWasSuccessful && !_response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT(" !bWasSuccessful && !_response.IsValid() "));
		return;
	}

	IImageWrapperModule& temp_img_module = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	IImageWrapperPtr temp_imgWrapper = temp_img_module.CreateImageWrapper(EImageFormat::JPEG);//JPG �� png ��

	TArray<uint8> temp_fileData = _response->GetContent();


	if (!temp_imgWrapper.IsValid() ||
		!temp_imgWrapper->SetCompressed(temp_fileData.GetData(), temp_fileData.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ImageWrapper can��t Set Compressed or ImageWrapper is InValid"));
		return;
	}

	const TArray<uint8>* temp_unCompressedRGBA = NULL;

	if (!temp_imgWrapper->GetRaw(ERGBFormat::RGBA, 8, temp_unCompressedRGBA))
	{
		UE_LOG(LogTemp, Warning, TEXT("can��t get Raw temp_unCompressedRGBA"));
		return;
	}

	Texture2D = UTexture2D::CreateTransient(temp_imgWrapper->GetWidth(), temp_imgWrapper->GetHeight());

	auto temp_dataPtr = static_cast<uint8*>(Texture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	FColor temp_color;
	uint8 temp_colorPoint;
	TArray<FColor> temp_arr_color;
	for (int i = 0; i < temp_unCompressedRGBA->Num(); i++)
	{
		temp_colorPoint = (*temp_unCompressedRGBA)[i];
		temp_color.R = temp_colorPoint;
		i++;
		temp_colorPoint = (*temp_unCompressedRGBA)[i];
		temp_color.G = temp_colorPoint;
		i++;
		temp_colorPoint = (*temp_unCompressedRGBA)[i];
		temp_color.B = temp_colorPoint;
		i++;
		temp_colorPoint = (*temp_unCompressedRGBA)[i];
		temp_color.A = temp_colorPoint;
		temp_arr_color.Add(temp_color);
	}

	uint8* DestPtr = NULL;
	const FColor* SrcPtr = NULL;
	for (int32 y = 0; y < temp_imgWrapper->GetHeight(); y++)
	{
		DestPtr = &temp_dataPtr[(temp_imgWrapper->GetHeight() - 1 - y) * temp_imgWrapper->GetWidth() * sizeof(FColor)];
		SrcPtr = const_cast<FColor*>(&temp_arr_color[(temp_imgWrapper->GetHeight() - 1 - y) * temp_imgWrapper->GetWidth()]);
		for (int32 x = 0; x < temp_imgWrapper->GetWidth(); x++)
		{
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->B;
			if (true)
			{
				*DestPtr++ = SrcPtr->A;
			}
			else
			{
				*DestPtr++ = 0xFF;
			}
			SrcPtr++;
		}
	}

	Texture2D->PlatformData->Mips[0].BulkData.Unlock();

	//Texture2D->UpdateResourceW();
	Texture2D->UpdateResource();

	//// ����ӿڲ���2
	//// -------------
	//FString urlOfCacheBlockCnt = AUrlsHandler::GetUrlOfGetBlockCacheCount();
	//TSharedRef<IHttpRequest> httpReuest2 = FHttpModule::Get().CreateRequest();
	//httpReuest2->SetVerb(TEXT("GET"));
	//httpReuest2->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	//httpReuest2->SetURL(urlOfCacheBlockCnt);
	//httpReuest2->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_CacheBlockCount);
	//httpReuest2->ProcessRequest();
}

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshComp");
	RootComponent = ProceduralMeshComp;

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOb(TEXT("Material'/Game/Materials/MTR_T1.MTR_T1'"));
	Material = MaterialOb.Object;

	//Texture2D'/Engine/EngineMaterials/DefaultBokeh.DefaultBokeh'
	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture2DOb(TEXT("Texture2D'/Engine/EngineMaterials/DefaultDiffuse.DefaultDiffuse'"));
	Texture2D = Texture2DOb.Object;


	//Texture2D

	/*static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInsOb(TEXT("MaterialInstanceConstant'/Game/MaterialInstances/MTR_T1_Inst.MTR_T1_Inst'"));
	MaterialInstance = MaterialInsOb.Object;*/
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
	AUrlsHandler::InitParameters(FString("46d11566-6b7e-47a1-ba5d-12761ab9b55c"), FString("67170069-1711-4f4c-8ee0-a715325942a1"), FString("69323"));
	FString allEleurls = AUrlsHandler::GetUrlOfGetAllElements();
	UE_LOG(LogTemp, Error, TEXT("allEleurls == %s"), *allEleurls);

	// ����ӿڲ���
	// ------------
	TSharedRef<IHttpRequest> httpReuest = FHttpModule::Get().CreateRequest();
	httpReuest->SetVerb(TEXT("GET"));
	httpReuest->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	httpReuest->SetURL(allEleurls);
	httpReuest->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_AllElementsInView);
	httpReuest->ProcessRequest();



	// ����ӿڲ���2
	// -------------
	FString urlOfCacheBlockCnt = AUrlsHandler::GetUrlOfGetBlockCacheCount();
	TSharedRef<IHttpRequest> httpReuest2 = FHttpModule::Get().CreateRequest();
	httpReuest2->SetVerb(TEXT("GET"));
	httpReuest2->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	httpReuest2->SetURL(urlOfCacheBlockCnt);
	httpReuest2->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_CacheBlockCount);
	httpReuest2->ProcessRequest();

	// �ȵ�������Ľӿڻ�ȡ UTexture2D ���ݣ�
	// -------------------------------------
	TSharedRef<IHttpRequest> httpReuestimg = FHttpModule::Get().CreateRequest();
	httpReuestimg->SetVerb(TEXT("GET"));
	httpReuestimg->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	httpReuestimg->SetURL(FString("https://bimcomposer.probim.cn/api/Model/GetFile?ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=67170069-1711-4f4c-8ee0-a715325942a1&VersionNO=&FileType=Texture&FileName=brickldnonlduniformldrunningldgrayppng"));
	httpReuestimg->OnProcessRequestComplete().BindUObject(this, &AMyActor::setTextureFromLoadImg);
	httpReuestimg->ProcessRequest();

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

void AMyActor::OnRequestComplete_AllElementsInView(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
		// �õ� res���õ��ӿڷ��ص��ַ���
		// ------------------------------
		IHttpResponse * res = Response.Get();
		FString ContentJson = Response->GetContentAsString();

		// �����ص��ַ���תΪ���֣���Ϊ��õ� block �ĸ���
		// -----------------------------------------------
		int blockFileCount = FCString::Atoi(*ContentJson);

		// ��1��ʼ�����ÿ�����֣��ٵ��� https://bimcomposer.probim.cn/api/Model/GetCacheBlock?FileID=1&ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=58080653-18d1-4067-b480-e02c56eb791a&VersionNO=&ViewID=168550
		// -------------------------------------------------------------------------------------------
		for (int i = 1; i <= blockFileCount; i++) 
		{
			UE_LOG(LogTemp, Warning, TEXT("to call GetCacheBlock?FileID= %d"), i);

			// �õ���ǰ�� url ��ַ
			// -------------------

			// ����ӿڲ���2
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

		// �õ���� block ���ص� buffer
		// ----------------------------
		TArray<uint8> resBuffer = Response.Get()->GetContent();
		UE_LOG(LogTemp, Warning, TEXT("buffer length is %d"), resBuffer.Num());

		// תΪ�ֽ�����
		// ------------
		BYTE* buffer = reinterpret_cast<BYTE*>(resBuffer.GetData());

		// ���÷�������������ֽ�����
		// --------------------------
		AMyActor::AnalysisBuffer(buffer, resBuffer.Num());
	}
}

// ctmLoadCustom �ص�
// ------------------
CTMuint CTMRreadfn(void* aBuf, CTMuint aCount, void* aUserData)
{
	static int count = 0;
	static void* ptr = nullptr;

	if (ptr != aUserData)
	{
		count = 0;
		ptr = aUserData;
	}

	memcpy((char*)aBuf, (char*)ptr + count, aCount * sizeof(char));
	count += aCount;

	return aCount;
}


// ��������ֽ�����
// ----------------
void AMyActor::AnalysisBuffer(BYTE * buffer, int size)
{
	int offset = 0;

	static int meshIndex = 0;

	// ���ڷ��ص����ݿ��ܲ�ֹ����һ�� |B365|
	// ������ѭ����ȡ��� blockBuffer
	// ------------------------------
	int looptimes = 0;
	while (offset < size)
	{
		// ���ѭ������
		// ------------
		UE_LOG(LogTemp, Warning, TEXT("looptimes : %d"), looptimes);

		looptimes++;

		// ����Ҫ���� |B365|
		// -----------------
		offset += strlen("|B365|");

		// ������¼ blockId �ĳ��ȵı���
		// -----------------------------
		int blockIdLength;

		// ������ȡ4���ֽڣ��洢�� blockIdLength ��
		// ȡ������ϼ�¼ƫ��λ��
		// ----------------------
		memcpy(&blockIdLength, buffer + offset, sizeof(int));
		offset += sizeof(int);

		// ������� blockIdLength ��ֵ
		// ---------------------------
		UE_LOG(LogTemp, Warning, TEXT("blockIdLength = %d"), blockIdLength);

		

		// ���� blockIdLength ���ֽڵ� BYTE�����ڴ洢 blockId ��ֵ
		// -------------------------------------------------------
		BYTE * blockIdBuffer = (BYTE*)calloc(blockIdLength, sizeof(BYTE));

		// ���� blockIdLength ���ֽڵ� blockIdBuffer
		// ȡ������ϼ�¼ƫ��λ��
		// ----------------------
		memcpy(blockIdBuffer, buffer + offset, blockIdLength);
		TCHAR* blockIdBuffer_utf8 = UTF8_TO_TCHAR(blockIdBuffer);
		offset += blockIdLength;

		if (strcmp("211665_499261", (char *)blockIdBuffer_utf8) == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("CONTAINS!"));
		}

		// ������� blockIdBuffer_utf8 ��ֵ
		// ---------------------------
		UE_LOG(LogTemp, Warning, TEXT("blockIdBuffer_utf8 = %s"), blockIdBuffer_utf8);

		// ������Ӧ���� |ID GEO| ����ַ��������������֤
		// ----------------------------------------------
		TCHAR ch[9] = { 0 };
		memcpy(ch, buffer + offset, 8);
		TCHAR * ch_uft8 = UTF8_TO_TCHAR(ch);
		UE_LOG(LogTemp, Warning, TEXT("ch_uft8 = %s"), ch_uft8);
		offset += strlen("|ID GEO|");

		// �ٶ�4���ֽڣ��õ�contentLength
		// ------------------------------
		int contentLength;
		memcpy(&contentLength, buffer + offset, sizeof(int));
		UE_LOG(LogTemp, Warning, TEXT("contentLength = %d"), contentLength);
		offset += 4;

		// �ȶ� contentLength ���ֽڣ��������ƫ��contentLength ���ֽڣ�������һ��ѭ��
		// ---------------------------------------------------------------------------
		BYTE * contentBuffer = (BYTE*)calloc(contentLength, sizeof(BYTE));
		memcpy(contentBuffer, buffer + offset, contentLength);
		offset += contentLength;

		// openCTM ��������
		// ----------------
		CTMcontext context = AOpenCTMHandler::m_CtmNewContext(CTMenum::CTM_IMPORT);
		if (context)
		{
			// ���� openCTM �� LoadCustom
			// --------------------------
			AOpenCTMHandler::m_CtmLoadCustom(context, CTMRreadfn, contentBuffer);

			// ���ø� api ��ȡ����
			// -------------------
			int vertex_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(context, CTMenum::CTM_VERTEX_COUNT) * VERTEX_3;
			UE_LOG(LogTemp, Warning, TEXT("vertex_comp_cnt = %d"), vertex_comp_cnt);

			int indicate_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(context, CTMenum::CTM_TRIANGLE_COUNT) * INDICATE_3; // ��������
			UE_LOG(LogTemp, Warning, TEXT("indicate_comp_cnt = %d"), indicate_comp_cnt);

			int uv_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(context, CTMenum::CTM_VERTEX_COUNT) * VERTEX_UV_2;
			UE_LOG(LogTemp, Warning, TEXT("uv_comp_cnt = %d"), uv_comp_cnt);

			// ��ȡ����
			// ��������
			// ��������
			// ��������
			// uv����
			// ------
			TArray<FVector> verticesArr;
			TArray<FVector> normalsArr;
			TArray<int32> indicesArr;
			TArray<FVector2D> uvsArr;
			TArray<FLinearColor> linearArr;

			// ��ȡ��������
			// ------------
			const CTMfloat * vertices = AOpenCTMHandler::m_CtmGetFloatArray(context, CTMenum::CTM_VERTICES);
			for (size_t i = 0; i < vertex_comp_cnt; i += VERTEX_3)
			{
				//UE4����ģ����������Ϊ��λ
				// ------------------------
				verticesArr.Add(FVector(vertices[i] * 100, vertices[i + 1] * 100, vertices[i + 2] * 100));
				//verticesArr.Add(FVector(vertices[i] * 10, vertices[i + 1] * 10, vertices[i + 2] * 10));

				linearArr.Add(FLinearColor(255.0f, 0, 0, 0.8));
			}

			// ��ȡ��������
			// ------------
			const CTMuint * indicates = AOpenCTMHandler::m_CtmGetIntegerArray(context, CTMenum::CTM_INDICES);
			for (size_t i = 0; i < indicate_comp_cnt; i++)
			{
				indicesArr.Add(indicates[i]);
			}

			// ��ȡuv����
			// ----------
			const CTMfloat * uvs = AOpenCTMHandler::m_CtmGetFloatArray(context, CTMenum::CTM_UV_MAP_1);
			for (size_t i = 0; i < uv_comp_cnt; i+= VERTEX_UV_2)
			{
				// ������Ϊ��λ
				// ------------
				uvsArr.Add(FVector2D(uvs[i] * 100, uvs[i + 1] * 100));
				//uvsArr.Add(FVector2D(uvs[i] * 1, uvs[i + 1] * 1));
			}

			// *��ȡ��������(vertex_comp_cnt �붥�㣨������������ͬ)
			// �ⲿ�ֿ��Բ������Ӱ����ա��������Ҫ���㷴��Ч��ʱ����Щ����������㡣����ķ���һ��������Ϊ���ڸõ����ڵ����������ɸõ����������������ɵ����������Ĳ�ˣ�һ���������������ַ�����ȷ��������˽���ٹ�һ����
			// �������飬����Ĵ�С�������񶥵����������
			// -----------------------------------------------------
			const CTMfloat * normals = AOpenCTMHandler::m_CtmGetFloatArray(context, CTMenum::CTM_NORMALS);
			for (size_t i = 0; i < vertex_comp_cnt; i+= VERTEX_3)
			{
				normalsArr.Add(FVector(normals[i], normals[i + 1], normals[i + 2]));
			}

			// �������
			// --------
			UE_LOG(LogTemp, Warning, TEXT("verticesArr's len: %d, normalArr's len: %d, indicesArr's len: %d, uvs's len: %d"), verticesArr.Num(), normalsArr.Num(), indicesArr.Num(), uvsArr.Num());

			// Create Mesh Section
			// -------------------
			// CreateMeshSection_LinearColor �Ƽ�ʹ�ô˷���
			int index = meshIndex;
			meshIndex++;
			ProceduralMeshComp->CreateMeshSection_LinearColor(index, verticesArr, indicesArr, normalsArr, uvsArr, linearArr, TArray<FProcMeshTangent>(), false);

			// ���ò���
			// --------
			auto MaterialInstanceDnm2 = UMaterialInstanceDynamic::Create(this->Material, this, FName(*(FString("MI_Dynamic") + FString::FromInt(index))));
			if (MaterialInstanceDnm2)
			{
				// ���������
				// ----------
				/*int randomI = FMath::RandHelper(255);
				UE_LOG(LogTemp, Warning, TEXT("randomI = %d"), randomI);*/

				// ͨ������ AV3 ���費ֵͬ
				// -----------------------
			/*	if (index % 3 == 1)
				{
					MaterialInstanceDnm2->SetVectorParameterValue("AV3", FLinearColor(255 * 1.0f, 0 * 1.0f, 0 * 1.0f, 1));
				}
				else if (index % 3 == 2)
				{
					MaterialInstanceDnm2->SetVectorParameterValue("AV3", FLinearColor(0 * 1.0f, 255 * 1.0f, 0 * 1.0f, 1));
				}
				else
				{
					MaterialInstanceDnm2->SetVectorParameterValue("AV3", FLinearColor(0 * 1.0f, 0 * 1.0f, 255 * 1.0f, 1));
				}*/
				
				
				// �� ��ͼ����� ����
				// ------------------
				if (Texture2D)
				{
					MaterialInstanceDnm2->SetTextureParameterValue("TV5", Texture2D);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Texture2D is NULL"));
				}

				// �����ʸ���� mesh ��Ƭ
				// ----------------------
				ProceduralMeshComp->SetMaterial(index, MaterialInstanceDnm2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MaterialInstanceDnm is null"));
			}

			// һ������
			// --------
			TArray<UMaterialInterface *> mtris;
			ProceduralMeshComp->GetUsedMaterials(mtris);
			UE_LOG(LogTemp, Warning, TEXT("mtris' num is %d"), mtris.Num());

			// ���ò���
			// --------
			/*if (index == 0)
			{
				static ConstructorHelpers::FObjectFinder<UMaterialInstance> TankStaticMesh(TEXT("MaterialInstanceConstant'/Engine/EditorMaterials/Utilities/LinearColorPicker_MATInst.LinearColorPicker_MATInst'"));
				if (TankStaticMesh.Succeeded())
				{
					ProceduralMeshComp->SetMaterial(index, TankStaticMesh.Object);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("TankStaticMesh is invalid"));
				}
			}*/
		

			// ֱ��ж��
			// --------
			AOpenCTMHandler::m_CtmFreeContext(context);
			context = NULL;
		}

		// д�� calloc������д free
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