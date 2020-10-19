// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

#include "../Common/JsonHandler.h"
#include "../Common/UrlsHandler.h"

#include "../Common/StringHandler.h"

#include "Interfaces/IHttpResponse.h"
#include "Misc/CString.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "../Model/FMaterialContent.h"
#include "UObject/ConstructorHelpers.h"
#include "ElementActor.h"

#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapperModule.h"
#include "../Model/FBlockMapping_Item.h"
#include "../Model/FModelInfo.h"

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

	//static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOb(TEXT("Material'/Game/Materials/MTR_T1.MTR_T1'"));
	//Material = MaterialOb.Object;

	////Texture2D'/Engine/EngineMaterials/DefaultBokeh.DefaultBokeh'
	//static ConstructorHelpers::FObjectFinder<UTexture2D> Texture2DOb(TEXT("Texture2D'/Engine/EngineMaterials/DefaultDiffuse.DefaultDiffuse'"));
	//Texture2D = Texture2DOb.Object;

	//����ŵ����캯����
	static ConstructorHelpers::FObjectFinder<UMaterial> baseColorMaterialAsset(TEXT("'Material'/Game/Materials/M_BaseColor.M_BaseColor''"));
	static ConstructorHelpers::FObjectFinder<UMaterial> textureMaterialAsset(TEXT("'Material'/Game/Materials/M_Texture.M_Texture''"));
	static ConstructorHelpers::FObjectFinder<UMaterial> translucentMaterialAsset(TEXT("'Material'/Game/Materials/M_Translucent.M_Translucent''"));

	if (baseColorMaterialAsset.Succeeded())
		pBaseColorMaterial = baseColorMaterialAsset.Object;

	if (textureMaterialAsset.Succeeded())
		pTextureMaterial = textureMaterialAsset.Object;

	if (translucentMaterialAsset.Succeeded())
		pTranslucentMaterial = translucentMaterialAsset.Object;


	//Texture2D

	/*static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInsOb(TEXT("MaterialInstanceConstant'/Game/MaterialInstances/MTR_T1_Inst.MTR_T1_Inst'"));
	MaterialInstance = MaterialInsOb.Object;*/
}

void AMyActor::CreateTranslucentMaterial(const FString materialId, FColor baseColor, FMaterialContent& content)
{
	//������͸������
	auto pMaterialInstance = UMaterialInstanceDynamic::Create(pTranslucentMaterial, this, FName(*materialId));

	//���û�����ɫ
	pMaterialInstance->SetVectorParameterValue("MainColor", baseColor);
	//����͸����
	pMaterialInstance->SetScalarParameterValue("Opacity", content.tp);
	//���ôֲڶ�
	pMaterialInstance->SetScalarParameterValue("Roughness", 1 - content.Glossness);

	dynamicMaterialMap.Emplace(materialId, pMaterialInstance);
}

void AMyActor::CreateBaseMaterial(const FString materialId, FColor baseColor, FMaterialContent& content)
{
	//������������
	auto pMaterialInstance = UMaterialInstanceDynamic::Create(pBaseColorMaterial, this, FName(*materialId));

	//���û�����ɫ
	pMaterialInstance->SetVectorParameterValue("MainColor", baseColor);

	//���ò��ʵĽ�����
	float metallicValue = content.ismetal == true ? 1.0f : 0.0f;
	pMaterialInstance->SetScalarParameterValue("Metallic", metallicValue);

	//���ôֲڶ�
	pMaterialInstance->SetScalarParameterValue("Roughness", 1 - content.Glossness);

	dynamicMaterialMap.Emplace(materialId, pMaterialInstance);
}

void AMyActor::CreateTextureMaterial(const FString materialId, FColor baseColor, FMaterialContent& content)
{

	//�����������
	auto pMaterialInstance = UMaterialInstanceDynamic::Create(pTextureMaterial, this, FName(*materialId));
	pMaterialInstance->SetTextureParameterValue("MainTexture", m_TextureMap[content.img]);

	//���������UVƫ��
	pMaterialInstance->SetScalarParameterValue("OffsetX", content.uf);
	pMaterialInstance->SetScalarParameterValue("OffsetY", content.vf);

	//���������UV����
	content.usc != 0.0f ? content.usc : 1.0f;
	content.vsc != 0.0f ? content.vsc : 1.0f;

	pMaterialInstance->SetScalarParameterValue("ScaleX", content.usc);
	pMaterialInstance->SetScalarParameterValue("ScaleY", content.vsc);

	//���û�����ɫ
	pMaterialInstance->SetVectorParameterValue("MainColor", baseColor);

	//���ò��ʵĽ�����
	float metallicValue = content.ismetal == true ? 1.0f : 0.0f;
	pMaterialInstance->SetScalarParameterValue("Metallic", metallicValue);

	//���ôֲڶ�
	pMaterialInstance->SetScalarParameterValue("Roughness", 1 - content.Glossness);

	dynamicMaterialMap.Emplace(materialId, pMaterialInstance);
}


// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// ��ʼ�� CTM �� content ���
	// [TODO]��δ�ͷ�
	// --------------
	this->context = AOpenCTMHandler::m_CtmNewContext(CTMenum::CTM_IMPORT);

	// ��� blockId �� materialId �� map
	// ---------------------------------
	m_MapBlockMaterial.Empty();

	// ��� m_MapMaterial ����
	// -----------------------
	m_MapMaterial.Empty();

	// ��� m_TextureNameSet ����
	// --------------------------
	m_TextureNameSet.Empty();
	m_TextureMap.Empty();

	// ������е� MeshSection !
	// ------------------------
	ProceduralMeshComp->ClearAllMeshSections();

	// ��ַƴ�Ӳ���
	// ------------
	// https://bimcomposer.probim.cn/api/Model/GetAllElementsInView?ProjectID=7d96928d-5add-45cb-a139-2c787141e50d&ModelID=9f49078c-180e-4dc5-b696-5a50a9e09016&VersionNO=&ViewID=142554
	//AUrlsHandler::InitParameters(FString("46d11566-6b7e-47a1-ba5d-12761ab9b55c"), FString("67170069-1711-4f4c-8ee0-a715325942a1"), FString("69323"));
	AUrlsHandler::InitParameters(FString("46d11566-6b7e-47a1-ba5d-12761ab9b55c"), FString("726f5ad8-0123-459f-8566-475146d396ef"), FString("142554"));
	
	// ����ӿڲ���1
	// ����GetModel
	// ------------
	FString urlOfGetModel = AUrlsHandler::GetUrlOfGetModel();
	TSharedRef<IHttpRequest> httpReuestGetModel = FHttpModule::Get().CreateRequest();
	httpReuestGetModel->SetVerb(TEXT("GET"));
	httpReuestGetModel->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
	httpReuestGetModel->SetURL(urlOfGetModel);
	httpReuestGetModel->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_GetModel);
	httpReuestGetModel->ProcessRequest();

	
}

void AMyActor::OnRequestComplete_GetModel(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Request.IsValid() || !Response.IsValid())
	{
		return;
	}

	if (bConnectedSuccessfully)
	{
		// �����ص�json���з����л�
		// ------------------------
		FString contentStr = Response->GetContentAsString();

		FModelInfo modelInfo;
		TSharedPtr<FJsonObject> tempParsed;
		bool tempFlag = AJsonHandler::Deserialize(contentStr, &tempParsed);
		if (tempFlag)
		{
			modelInfo.ID = tempParsed.Get()->GetStringField(FString("ID"));
			modelInfo.Name = tempParsed.Get()->GetStringField(FString("Name"));
			modelInfo.ProjectID = tempParsed.Get()->GetStringField(FString("ProjectID"));
			modelInfo.Views = tempParsed.Get()->GetArrayField(FString("Views"));

			for (size_t i = 0; i < modelInfo.Views.Num(); i++)
			{
				auto & item = modelInfo.Views[i];

				FViewInfo viewinfo;
				viewinfo.ID = AJsonHandler::TryGetStringField(item, FString("ID"), FString(""));
				viewinfo.IsDefault = AJsonHandler::TryGetBoolField(item, FString("IsDefault"), false);
				viewinfo.ModelID = AJsonHandler::TryGetStringField(item, FString("ModelID"), FString(""));
				viewinfo.Name = AJsonHandler::TryGetStringField(item, FString("Name"), FString(""));
				viewinfo.Override = AJsonHandler::TryGetStringField(item, FString("Override"), FString(""));

				TSharedPtr<FJsonObject> tempParsed2;
				bool tempFlag2 = AJsonHandler::Deserialize(viewinfo.Override, &tempParsed2);
				if (tempFlag2)
				{
					viewinfo.Override_obj.displaystyle = AJsonHandler::TryGetStringField_FromObj(tempParsed2, FString("displaystyle"), FString(""));
				}

				modelInfo.Views_objlist.Add(viewinfo);
			}

			// ����������ͼ���ҵ��Ǹ�Ĭ����ͼ�������м��أ�
			// -------------------------------------------
			ReadViewInfos(modelInfo.Views_objlist);
		}

		UE_LOG(LogTemp, Warning, TEXT("214: %s"), *contentStr);
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

						// map set
						// -------
						m_MapBlockMaterial.Add(blockMappingItem.blockId, blockMappingItem.materialId);

						// blockMappingItems => blockMappingItems_objlist
						// ----------------------------------------------
						ele.BlockMapping_obj.blockMappingItems_objlist.Add(blockMappingItem);
					}
				}

				eles.Add(ele);
			}

			// ��ӡ��� eles �ĸ���
			// --------------------
			UE_LOG(LogTemp, Warning, TEXT("eles' count is %d"), eles.Num());
			//UE_LOG(LogTemp, Error, TEXT("7 ---- %d"), (eles[eles.Num() - 1].BlockMapping_obj.blockMappingItems_objlist[0].transform_list.Num()));

			// ������Щ eles�������� Spawn Actor
			// ---------------------------------
			SpawnActorsByElements(eles);

			UE_LOG(LogTemp, Warning, TEXT("begin to free"));

			for (size_t i = 0; i < contentBufferFromCTMAnalysis.Num(); i++)
			{
				if (contentBufferFromCTMAnalysis[i])
				{
					free(contentBufferFromCTMAnalysis[i]);
				}
			}
			contentBufferFromCTMAnalysis.Empty();
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
		this->m_blockFileCount = blockFileCount;

		// ��һ�� meshSectionMap
		// ---------------------
		meshSectionMap.Empty();

#if 0
		// �ȵ��� GetMaterials
		// -------------------
		FString urlOfMaterials = AUrlsHandler::GetUrlOfGetMaterials();
		TSharedRef<IHttpRequest> httpReuestMtr = FHttpModule::Get().CreateRequest();
		httpReuestMtr->SetVerb(TEXT("GET"));
		httpReuestMtr->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
		httpReuestMtr->SetURL(urlOfMaterials);
		httpReuestMtr->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_GetMaterials);
		httpReuestMtr->ProcessRequest();
#endif

		// ��1��ʼ�����ÿ�����֣��ٵ��� https://bimcomposer.probim.cn/api/Model/GetCacheBlock?FileID=1&ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=58080653-18d1-4067-b480-e02c56eb791a&VersionNO=&ViewID=168550
		// -------------------------------------------------------------------------------------------
		for (int i = 1; i <= m_blockFileCount; i++)
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
		// ���湹���� meshSectionMap
		// --------------------------
		AMyActor::AnalysisBuffer(buffer, resBuffer.Num());

		this->m_blockFileRead++;

		// �ж�������������һ�ε�block file�ļ�����ʼ����GetAllElementsInView
		// ----------------------------------------------------------------------
		if (this->m_blockFileRead == this->m_blockFileCount)
		{
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
		}
	}
}

void AMyActor::OnRequestComplete_GetTexture(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Request.IsValid() || !Response.IsValid())
	{
		return;
	}

	if (bConnectedSuccessfully)
	{
		TArray<uint8> imageData = Response->GetContent();
		UE_LOG(LogTemp, Warning, TEXT("imageData's length = %d"), imageData.Num());

		//��URL����ȡ���������
		// --------------------
		FString fileName = Request->GetURLParameter("FileName");

		// ���� buffer ���� Texture������ӵ� TextureMap
		// ---------------------------------------------
		CreateTexture(&imageData, fileName);
		this->m_TextureCount++;

		// ȫ������������ɺ󣬴�������
		// ----------------------------
		if (this->m_TextureNameSet.Num() == m_TextureCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("Begin to Create Material"));

			// �������ʲ���ӵ� dynamicMaterialMap
			// -----------------------------------
			for (auto material : this->m_MapMaterial)
			{
				FMaterialContent materialContent = material.Value.Content_Obj;

				FString colorHex = materialContent.tinttoggle ? (!materialContent.tintcolor.IsEmpty() ? materialContent.tintcolor : TEXT("d6f5fc"))
					: (this->m_HasApperenanceColor ? (!materialContent.appearancecolor.IsEmpty() ? materialContent.appearancecolor : TEXT("d6f5fc"))
						: (!materialContent.color.IsEmpty() ? materialContent.color : TEXT("d6f5fc")));

				colorHex = TEXT("#") + colorHex;

				auto color = FColor::FromHex(colorHex);

				//UMaterialInstanceDynamic* pMaterialInstance;
				FString imgName = materialContent.img;

				//�ж��Ƿ��ǰ�͸������
				if (materialContent.tp < 1.0f)
				{
					//������͸������
					CreateTranslucentMaterial(material.Key, color, materialContent);
				}
				else
				{
					if (!imgName.IsEmpty() && this->m_TextureMap.Contains(imgName))
					{
						//�����������
						CreateTextureMaterial(material.Key, color, materialContent);
					}
					else
					{
						//����������ɫ����
						CreateBaseMaterial(material.Key, color, materialContent);
					}
				}

			}
		
			// ���� blockCount �Ⱥ�������
			// --------------------------
			// ����ӿڲ���2
			// �����ŵ� GetModel�� �ص���
			// --------------------------
			FString urlOfCacheBlockCnt = AUrlsHandler::GetUrlOfGetBlockCacheCount();
			TSharedRef<IHttpRequest> httpReuest2 = FHttpModule::Get().CreateRequest();
			httpReuest2->SetVerb(TEXT("GET"));
			httpReuest2->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
			httpReuest2->SetURL(urlOfCacheBlockCnt);
			httpReuest2->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_CacheBlockCount);
			httpReuest2->ProcessRequest();
		}

	}
}

void AMyActor::CreateTexture(TArray<uint8>* buffer, const FString textureName, EImageFormat defaultF)
{
	EImageFormat imageFormat = defaultF;

	if (textureName.EndsWith(TEXT("ppng")))
	{
		imageFormat = EImageFormat::JPEG;
	}
	else if (textureName.EndsWith(TEXT("pjpg")))
	{
		imageFormat = EImageFormat::PNG;
	}
	else
		imageFormat = EImageFormat::Invalid;

	UE_LOG(LogTemp, Error, TEXT("%s"), *textureName);

	if (imageFormat == EImageFormat::Invalid)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Invalid or unrecognized format"), *textureName);
		return;
	}

	IImageWrapperModule& imageModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper>  pImageWrapper = imageModule.CreateImageWrapper(imageFormat);

	if (!pImageWrapper.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s ImageWrapper  is InValid"), *textureName);
		return;
	}

	if (!pImageWrapper->SetCompressed(buffer->GetData(), buffer->Num()))
	{
		//CreateTexture(buffer, textureName);
		if (defaultF != EImageFormat::PNG)
		{
			CreateTexture(buffer, textureName, EImageFormat::PNG);
		}

		UE_LOG(LogTemp, Warning, TEXT("%s ImageWrapper can't Set Compressed "), *textureName);
		return;
	}

	const TArray<uint8>* unCompressedRGBA = nullptr;

	if (!pImageWrapper->GetRaw(ERGBFormat::BGRA, 8, unCompressedRGBA))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s can't get Raw unCompressedRGBA"), *textureName);
		return;
	}

	//����Texture2D����
	auto pTexture = UTexture2D::CreateTransient(pImageWrapper->GetWidth(), pImageWrapper->GetHeight(), PF_B8G8R8A8, FName(*textureName));

	//��ס�������ݣ��Ա��޸�
	auto pTextureData = static_cast<uint8*>(pTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	FMemory::Memcpy(pTextureData, unCompressedRGBA->GetData(), unCompressedRGBA->Num());

	//��������
	pTexture->PlatformData->Mips[0].BulkData.Unlock();
	pTexture->UpdateResource();

	if (!this->m_TextureMap.Contains(textureName))
	{
		this->m_TextureMap.Emplace(textureName, pTexture);
	}
	//UE_LOG(LogTemp,Error,TEXT("Load TextureName:%s"), *textureName);
}

void AMyActor::OnRequestComplete_GetMaterials(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Request.IsValid() || !Response.IsValid())
	{
		return;
	}

	if (bConnectedSuccessfully)
	{
		FString allMaterials = Response->GetContentAsString();

		// �� allMaterials �����л�Ϊʵ��
		// ------------------------------
		TArray<TSharedPtr<FJsonValue>> originParsed;
		bool BFlag = AJsonHandler::DeserializeList(allMaterials, &originParsed);
		if (BFlag)
		{
			//// ��ͼ���Ƶ� Set
			//// --------------
			//TSet<FString> textureNameSet;
			//int textureCount;

			for (size_t i = 0; i < originParsed.Num(); i++)
			{
				FMaterial2 mtr;
				mtr.ID = AJsonHandler::TryGetStringField(originParsed[i], FString("ID"), FString(""));
				mtr.Content = AJsonHandler::TryGetStringField(originParsed[i], FString("Content"), FString(""));

				TSharedPtr<FJsonObject> JsonObject;
				bool bIsOk = AJsonHandler::Deserialize(mtr.Content, &JsonObject);
				if (bIsOk)
				{
					mtr.Content_Obj.ang = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("ang"), 0.0f);
					mtr.Content_Obj.appearancecolor = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("appearancecolor"), FString(""));
					mtr.Content_Obj.cat = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("cat"), FString(""));
					mtr.Content_Obj.color = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("color"), FString(""));
					mtr.Content_Obj.Glossness = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("Glossness"), 0.0f);
					mtr.Content_Obj.img = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("img"), FString(""));
					mtr.Content_Obj.ismetal = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("ismetal"), false);
					JsonObject->TryGetStringArrayField(FString("othertextures"), mtr.Content_Obj.othertextures);
					mtr.Content_Obj.ReflectivityDirect = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("ReflectivityDirect"), 0.0f);
					mtr.Content_Obj.ReflectivityOblique = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("ReflectivityOblique"), 0.0f);
					mtr.Content_Obj.rpc = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("rpc"), false);
					mtr.Content_Obj.rpccat = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("rpccat"), FString(""));
					mtr.Content_Obj.rpcheight = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("rpcheight"), 0.0f);
					mtr.Content_Obj.rpcid = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("rpcid"), FString(""));
					mtr.Content_Obj.rpcreflections = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("rpcreflections"), false);
					mtr.Content_Obj.rpcshadows = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("rpcshadows"), false);
					mtr.Content_Obj.SelfIllumColorTemperature = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("SelfIllumColorTemperature"), 0.0f);
					mtr.Content_Obj.SelfIllumFiltermap = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("SelfIllumFiltermap"), FString(""));
					mtr.Content_Obj.SelfIllumLuminance = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("SelfIllumLuminance"), 0.0f);
					mtr.Content_Obj.shi = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("shi"), 0);
					mtr.Content_Obj.tintcolor = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("tintcolor"), FString(""));
					mtr.Content_Obj.tinttoggle = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("tinttoggle"), false);
					mtr.Content_Obj.tp = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("tp"), 0.0f);
					mtr.Content_Obj.typ = AJsonHandler::TryGetStringField_FromObj(JsonObject, FString("typ"), FString(""));
					mtr.Content_Obj.uf = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("uf"), 0.0f);
					mtr.Content_Obj.URepeat = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("URepeat"), false);
					mtr.Content_Obj.usc = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("usc"), 0.0f);
					mtr.Content_Obj.userenderappearance = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("userenderappearance"), false);
					mtr.Content_Obj.uvsc = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("uvsc"), 0.0f);
					mtr.Content_Obj.vf = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("vf"), 0.0f);
					mtr.Content_Obj.VRepeat = AJsonHandler::TryGetBoolField_FromObj(JsonObject, FString("VRepeat"), false);
					mtr.Content_Obj.vsc = AJsonHandler::TryGetNumberField_FromObj(JsonObject, FString("vsc"), 0.0f);
				}

				//m_Materials.Add(mtr);
				// ��Ӳ���id �� ���ݵ� map
				// ------------------------
				m_MapMaterial.Add(mtr.ID, mtr);

				// �������ͼ��������Ƶ� Set ��
				// -----------------------------
				if (!mtr.Content_Obj.img.IsEmpty())
				{
					m_TextureNameSet.Add(mtr.Content_Obj.img);
				}
			}

			// ���� m_TextureNameSet����������ͼ
			// -------------------------------
			for (auto textureName : m_TextureNameSet)
			{
				FString urlOfGetTexture = AUrlsHandler::GetUrlOfGetTextureFile(textureName);
				TSharedRef<IHttpRequest> httpReuest = FHttpModule::Get().CreateRequest();
				httpReuest->SetVerb(TEXT("GET"));
				httpReuest->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
				httpReuest->SetURL(urlOfGetTexture);
				httpReuest->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_GetTexture);
				httpReuest->ProcessRequest();
			}

		}
		
	}
}



void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FMaterialSection * section;
	//bool bDequeueSuc = this->MaterialSectionQueue.Dequeue(section);
	//if (bDequeueSuc)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Dequeue suc!"));

	//	// �� ��ͼ����� ����
	//	// ------------------
	//	if (section->Texture2D)
	//	{
	//		//133 7167 3776
	//		section->m_MaterialInstanceDyn->SetTextureParameterValue("TV5", section->Texture2D);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Texture2D is NULL"));
	//	}

	//	// �����ʸ���� mesh ��Ƭ
	//	// ----------------------
	//	section->m_ProceduralMeshComp->SetMaterial(section->m_MaterialSectionIndex, section->m_MaterialInstanceDyn);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Dequeue no data!"));
	//}
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

	int blockloaded = 0;

	// ���ڷ��ص����ݿ��ܲ�ֹ����һ�� |B365|
	// ������ѭ����ȡ��� blockBuffer
	// ------------------------------
	while (offset < size)
	{
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

		// ���� blockIdLength ���ֽڵ� BYTE�����ڴ洢 blockId ��ֵ
		// -------------------------------------------------------
		BYTE * blockIdBuffer = (BYTE*)calloc(blockIdLength, sizeof(BYTE));

		// ���� blockIdLength ���ֽڵ� blockIdBuffer
		// ȡ������ϼ�¼ƫ��λ��
		// ----------------------
		memcpy(blockIdBuffer, buffer + offset, blockIdLength);
		TCHAR* blockIdBuffer_utf8 = UTF8_TO_TCHAR(blockIdBuffer);
		offset += blockIdLength;

		// ������Ӧ���� |ID GEO| ����ַ��������������֤
		// ----------------------------------------------
		TCHAR ch[9] = { 0 };
		memcpy(ch, buffer + offset, 8);
		TCHAR * ch_uft8 = UTF8_TO_TCHAR(ch);
		offset += strlen("|ID GEO|");

		// �ٶ�4���ֽڣ��õ�contentLength
		// ------------------------------
		int contentLength;
		memcpy(&contentLength, buffer + offset, sizeof(int));
		offset += 4;

		// �ȶ� contentLength ���ֽڣ��������ƫ��contentLength ���ֽڣ�������һ��ѭ��
		// ---------------------------------------------------------------------------
		BYTE * contentBuffer = (BYTE*)calloc(contentLength, sizeof(BYTE));
		memcpy(contentBuffer, buffer + offset, contentLength);
		offset += contentLength;
		
		if (this->context)
		{
			// ���� openCTM �� LoadCustom
			// --------------------------
			AOpenCTMHandler::m_CtmLoadCustom(this->context, CTMRreadfn, contentBuffer);

			// ���ø� api ��ȡ����
			// -------------------
			int vertex_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(this->context, CTMenum::CTM_VERTEX_COUNT) * VERTEX_3;
			int indicate_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(this->context, CTMenum::CTM_TRIANGLE_COUNT) * INDICATE_3; // ��������
			int uv_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(this->context, CTMenum::CTM_VERTEX_COUNT) * VERTEX_UV_2;

			// ��ȡ����
			// ��������
			// ��������
			// ��������
			// uv����
			// ------
			TArray<FVector> verticesArr;
			TArray<FVector> normalsArr;
			TArray<uint32> indicesArr;
			TArray<FVector2D> uvsArr;
			//TArray<FLinearColor> linearArr;

			// ��ȡ��������
			// ------------
			const CTMfloat * vertices = AOpenCTMHandler::m_CtmGetFloatArray(this->context, CTMenum::CTM_VERTICES);
			for (size_t i = 0; i < vertex_comp_cnt; i += VERTEX_3)
			{
				//UE4����ģ����������Ϊ��λ
				//verticesArr.Add(FVector(vertices[i] * 10, vertices[i + 1] * 10, vertices[i + 2] * 10));
				//linearArr.Add(FLinearColor(255.0f, 0, 0, 0.8));
				// ----------------------------------------------
				verticesArr.Add(FVector(vertices[i] * 100, vertices[i + 1] * -100, vertices[i + 2] * 100));
			}

			// ��ȡ��������
			// ------------
			const CTMuint * indicates = AOpenCTMHandler::m_CtmGetIntegerArray(this->context, CTMenum::CTM_INDICES);
			for (size_t i = 0; i < indicate_comp_cnt; i++)
			{
				indicesArr.Add(indicates[i]);
			}

			// ��ȡuv����
			// ----------
			const CTMfloat * uvs = AOpenCTMHandler::m_CtmGetFloatArray(this->context, CTMenum::CTM_UV_MAP_1);
			if (uvs)
			{
				for (size_t i = 0; i < uv_comp_cnt; i += VERTEX_UV_2)
				{
					// ������Ϊ��λ
					// ------------
					uvsArr.Add(FVector2D(uvs[i], uvs[i + 1]));
					//uvsArr.Add(FVector2D(uvs[i] * 1, uvs[i + 1] * 1));
				}
			}

			// *��ȡ��������(vertex_comp_cnt �붥�㣨������������ͬ)
			// �ⲿ�ֿ��Բ������Ӱ����ա��������Ҫ���㷴��Ч��ʱ����Щ����������㡣����ķ���һ��������Ϊ���ڸõ����ڵ����������ɸõ����������������ɵ����������Ĳ�ˣ�һ���������������ַ�����ȷ��������˽���ٹ�һ����
			// �������飬����Ĵ�С�������񶥵����������
			// -----------------------------------------------------
			const CTMfloat * normals = AOpenCTMHandler::m_CtmGetFloatArray(this->context, CTMenum::CTM_NORMALS);
			if (normals)
			{
				for (size_t i = 0; i < vertex_comp_cnt; i += VERTEX_3)
				{
					normalsArr.Add(FVector(normals[i], normals[i + 1] , normals[i + 2]));
				}
			}

			// !
			// ���� FProcMeshSection
			// һ��blockFile�е����ݿ��ܰ���һ������block���������
			// ÿһ��blockId��Ӧһ��meshSection
			// --------------------------------
			FProcMeshSection meshSection;
			/* �����ζ������� */
			meshSection.ProcIndexBuffer = indicesArr;
			/* �������� */
			meshSection.ProcVertexBuffer = ConvertVertexToProcVertex(verticesArr, uvsArr);
			/* bSectionVisible �� bEnableCollision */
			meshSection.bSectionVisible = true;
			meshSection.bEnableCollision = false;

			// ��ӵ� meshSectionMap
			// ---------------------
			FString blockId = FString(blockIdBuffer_utf8);
			meshSectionMap.Emplace(blockId, meshSection);

			//// ��ӵ�����
			//// ----------
			//FMaterialSection * section = new FMaterialSection;
			//section->m_blockId = FString(blockIdBuffer_utf8);
			//if (this->m_MapBlockMaterial.Contains(section->m_blockId))
			//{
			//	section->m_MaterialId = this->m_MapBlockMaterial[section->m_blockId];
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Error, TEXT("m_MapBlockMaterial doesn't contains m_blockId: %s"), *section->m_blockId);
			//}
			//section->m_ProceduralMeshComp = ProceduralMeshComp;
			//section->Material = this->Material;
			//section->m_MaterialSectionIndex = MaterialSectionArr.Num();
			//section->indicesArr = indicesArr;
			//section->verticesArr = verticesArr;
			//section->normalsArr = normalsArr;
			//section->uvsArr = uvsArr;
			//MaterialSectionArr.Add(section);

			// ֱ��ж��
			// --------
			//AOpenCTMHandler::m_CtmFreeContext(context);
			//context = NULL;
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
			/*free(contentBuffer);
			contentBuffer = NULL;*/
			contentBufferFromCTMAnalysis.Add(contentBuffer);
		}

		blockloaded++;
		/*if (blockloaded == 32)
		{
			break;
		}*/
	}

#if 0
	// ������ԣ�һ���ж��ٸ� Mesh ��Ƭ
// --------------------------------
	int sectionNum = MaterialSectionArr.Num(); //ProceduralMeshComp->GetNumSections();
	UE_LOG(LogTemp, Warning, TEXT("[721:7]MaterialSectionArr's Num is %d"), sectionNum);

	// test
	// ----
	int mapnum = m_MapBlockMaterial.Num();
	UE_LOG(LogTemp, Warning, TEXT("map's num is %d"), mapnum);

	// ���ԣ����������� Texture2D
	// --------------------------
	for (size_t i = 0; i < sectionNum; i++)
	{
		//// �� m_MapMaterial �ҵ� materialId ��Ӧ������
		//// -----------------------------------------
		//auto materialId = this->m_MapBlockMaterial[MaterialSectionArr[i]->m_blockId];

		//FMaterial2 & mtrObj = m_MapMaterial[materialId];
		//UE_LOG(LogTemp, Warning, TEXT("[759]color is %s"), *(mtrObj.Content_Obj.appearancecolor));

		// ����Actor
		// ---------
		//UClass* pBlueprintClass = StaticLoadClass(AElementActor::StaticClass(), nullptr, TEXT("Blueprint'/Game/Blueprints/Tests/BP_ElementActor.BP_ElementActor_C'"));
		UClass* pBlueprintClass = StaticLoadClass(AElementActor::StaticClass(), this, TEXT("Blueprint'/Game/Blueprints/Tests/BP_ElementActor.BP_ElementActor_C'"));

		if (pBlueprintClass)
		{
			// ��̬���һ�� Actor
			// ------------------
			AElementActor * theActor = GetWorld()->SpawnActor<AElementActor>(pBlueprintClass);

			// Ϊ�������ӵ� Actor ���� Mesh ��Ƭ
			// ÿ��Actorֻ����һ����Ƭ
			// -----------------------
			theActor->ProceduralMeshComp->CreateMeshSection_LinearColor(0, MaterialSectionArr[i]->verticesArr, MaterialSectionArr[i]->indicesArr, MaterialSectionArr[i]->normalsArr, MaterialSectionArr[i]->uvsArr, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);

			//// ������̬����ʵ��
			//// ----------------
			//auto dynMaterial = UMaterialInstanceDynamic::Create(theActor->Material, theActor, FName(*(FString("Name_") + FString::FromInt(0))));
			//theActor->m_MaterialInstanceDyn = dynMaterial;

			//// �޸�������ʵ�һ������
			//// ----------------------
			//dynMaterial->SetVectorParameterValue("AV3", FLinearColor(0.4f, 0.4f, 0.4f, 1));

			//// ���õ���� actor �� Mesh ��
			//// ---------------------------
			//theActor->ProceduralMeshComp->SetMaterial(0, dynMaterial);

			//// ���ýӿڣ���ȡ��ͼ
			//// ------------------
			//if (mtrObj.Content_Obj.img != FString(""))
			//{
			//	TSharedRef<IHttpRequest> httpReuestimg = FHttpModule::Get().CreateRequest();
			//	httpReuestimg->SetVerb(TEXT("GET"));
			//	httpReuestimg->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
			//	httpReuestimg->SetURL(AUrlsHandler::GetUrlOfGetTextureFile(mtrObj.Content_Obj.img));
			//	httpReuestimg->OnProcessRequestComplete().BindUObject(theActor, &AElementActor::setTextureFromLoadImg3);
			//	httpReuestimg->ProcessRequest();
			//}

			//continue;
		}

		//// Create Mesh Section
		//// CreateMeshSection_LinearColor �Ƽ�ʹ�ô˷���
		//// --------------------------------------------
		//MaterialSectionArr[i]->m_ProceduralMeshComp->CreateMeshSection_LinearColor(i, MaterialSectionArr[i]->verticesArr, MaterialSectionArr[i]->indicesArr, MaterialSectionArr[i]->normalsArr, MaterialSectionArr[i]->uvsArr, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);

		//// ָ�����
		//// --------
		//MaterialSectionArr[i]->m_MaterialInstanceDyn = UMaterialInstanceDynamic::Create(this->Material, this, FName(*(FString("MI_Dynamic") + FString::FromInt(i))));

		//// ���ò��ʲ��� ��ɫ
		//// -----------------
		///*	MaterialSectionArr[i]->m_MaterialInstanceDyn->SetVectorParameterValue("AV3", FLinearColor(100.0f, 0,0, 0.2));*/

		//// ���ýӿڣ���ȡ��ͼ
		//// ------------------
		//if (mtrObj.Content_Obj.img != FString(""))
		//{
		//	TSharedRef<IHttpRequest> httpReuestimg = FHttpModule::Get().CreateRequest();
		//	httpReuestimg->SetVerb(TEXT("GET"));
		//	httpReuestimg->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
		//	httpReuestimg->SetURL(AUrlsHandler::GetUrlOfGetTextureFile(mtrObj.Content_Obj.img));
		//	httpReuestimg->OnProcessRequestComplete().BindRaw(MaterialSectionArr[i], &FMaterialSection::setTextureFromLoadImg2);
		//	httpReuestimg->ProcessRequest();

		//	// ���ò��ʲ��� ��ͼ
		//	// -----------------
		//	//if (tmpTexture)//MaterialSectionArr[i]->Texture2D)
		//	//{
		//	//	MaterialSectionArr[i]->m_MaterialInstanceDyn->SetTextureParameterValue("TV5", tmpTexture);// MaterialSectionArr[i]->Texture2D);
		//	//}
		//}

		//// ���ò��ʵ� Mesh ���
		//// --------------------
		//MaterialSectionArr[i]->m_ProceduralMeshComp->SetMaterial(MaterialSectionArr[i]->m_MaterialSectionIndex, MaterialSectionArr[i]->m_MaterialInstanceDyn);

	}
#endif

	//// ����
	//// -----------------------------------------------------------------------------------------------------
	//TArray<UMaterialInterface *> mtris;
	//ProceduralMeshComp->GetUsedMaterials(mtris);
	//UE_LOG(LogTemp, Warning, TEXT("mtris' num is %d"), mtris.Num());
}

void AMyActor::SpawnActorsByElements(TArray<FElement> eles)
{


	UClass* pBlueprintClass = StaticLoadClass(AElementActor::StaticClass(), this, TEXT("Blueprint'/Game/Blueprints/Tests/BP_ElementActor.BP_ElementActor_C'"));
	if (pBlueprintClass)
	{
		for (size_t i = 0; i < eles.Num(); i++)
		{
			// �õ����element �е� blockMappingItems
			// --------------------------------------
			TArray<FBlockMapping_Item> items = eles[i].BlockMapping_obj.blockMappingItems_objlist;

			// ������Щ items
			// --------------
			for (size_t j = 0; j < items.Num(); j++)
			{
				// ����һ�� Spawn ����
				// -------------------
				FActorSpawnParameters SpawnPara;
				SpawnPara.Name = FName(*(eles[i].ElementID + TEXT("_") + FString::FromInt(j)));

				// ͨ�� items[j].transform_list �õ� Spawn ���õ��ľ������
				// --------------------------------------------------------
				FTransform actorTransform = GetElementTransform(items[j].transform_list);

				// ���� Spawn
				// ----------
				AElementActor* pElementActor = GetWorld()->SpawnActor<AElementActor>(pBlueprintClass, actorTransform, SpawnPara);

				// ���� loadMesh
				// -------------
			/*	FProcMeshSection* mesh = meshSectionMap.Find(items[j].blockId);
				pElementActor->ProceduralMeshComp->SetProcMeshSection(0, *mesh);
				pElementActor->ProceduralMeshComp->UpdateBounds();*/

#if 1 // δ���� Mesh ��

				FProcMeshSection* mesh = meshSectionMap.Find(items[j].blockId);
				auto vertexBuffer = mesh->ProcVertexBuffer;
				TArray<FVector> positions;
				TArray<FVector2D> uvos;
				TArray<int32> indexs;
				for (int32 k = 0; k < mesh->ProcIndexBuffer.Num(); k++)
				{
					indexs.Emplace(mesh->ProcIndexBuffer[k]);
				}
				for (int32 k = 0; k < vertexBuffer.Num(); k++)
				{
					positions.Emplace(vertexBuffer[k].Position);
					uvos.Emplace(vertexBuffer[k].UV0);
				}
				pElementActor->ProceduralMeshComp->CreateMeshSection_LinearColor(0, positions, indexs, TArray<FVector>(), uvos, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
#endif

				// loadMesh
				// --------
			/*	FProcMeshSection* mesh = ABlockHandlerActor::GetMeshMap()->Find(items[i].blockId);
				UMaterialInstanceDynamic* material = AMaterialHandlerActor::GetMaterialMap()[items[i].materialId];
				pElementActor->LoadMesh(0, mesh, material);*/

				UMaterialInstanceDynamic * material = this->dynamicMaterialMap[items[j].materialId];
				pElementActor->ProceduralMeshComp->SetMaterial(0, material);
				pElementActor->ProceduralMeshComp->UpdateBounds();

				
			}
		}
	}
}

// �� TArray<FVector> תΪ FProcMeshSection�����ڲ���Ҫ������
// ----------------------------------------------------------
TArray<FProcMeshVertex> AMyActor::ConvertVertexToProcVertex(TArray<FVector> vertex, TArray<FVector2D> uvs)
{
	TArray<FProcMeshVertex> retPMV;
	for (size_t i = 0; i < vertex.Num(); i++)
	{
		FProcMeshVertex pmvertex;
		pmvertex.Position = vertex[i];
		//vertex.Normal = normals[i];
		if (uvs.Num() != 0)
		{
			pmvertex.UV0 = uvs[i];
		}
		retPMV.Add(pmvertex);
	}
	return retPMV;
}

FTransform AMyActor::GetElementTransform(TArray<double> transform_list)
{
	FMatrix mat;
	//mat.M[0][0] = transform_list[0];
	//mat.M[0][1] = transform_list[1];
	//...
	//mat.M[3][3] = transform_list[15];
	for (int32 i = 0; i < 4; i++)
	{
		for (int32 j = 0; j < 4; j++)
		{
			mat.M[i][j] = transform_list[4 * i + j];
		}
	}

	mat = mat.GetTransposed();

	//UE�е����굥λΪ���ף����������ݵ����굥λΪ��
	mat.M[3][0] *= 100;
	mat.M[3][1] *= -100;
	mat.M[3][2] *= 100;

	//mat.M[0][3] *= 100;
	//mat.M[1][3] *= 100;
	//mat.M[2][3] *= 100;

	return FTransform(mat);
}

// ����������ͼ���ҵ��Ǹ�Ĭ����ͼ�������м��أ�
// -------------------------------------------
void AMyActor::ReadViewInfos(TArray<FViewInfo> viewInfos)
{
	for (int i = 0; i < viewInfos.Num(); i++)
	{
		if (viewInfos[i].IsDefault)
		{
			// TODO what?
			// ----------
			TArray<FString> overrides;
			overrides.Emplace(TEXT("Rendering"));
			overrides.Emplace(TEXT("Realistic"));
			overrides.Emplace(TEXT("RealisticWithEdges"));
			bool hasApperenanceColor;
			if ((!viewInfos[i].Override_obj.displaystyle.IsEmpty()) && (overrides.IndexOfByKey(viewInfos[i].Override_obj.displaystyle) != INDEX_NONE))
			{
				hasApperenanceColor = true;
			}
			else
			{
				hasApperenanceColor = false;
			}
			this->m_HasApperenanceColor = hasApperenanceColor;

			// ���ز��ʣ��ڲ��õ� m_HasApperenanceColor
			// ----------------------------------------
			FString urlOfGetMaterials = AUrlsHandler::GetUrlOfGetMaterials();
			TSharedRef<IHttpRequest> httpReuestGetMaterials = FHttpModule::Get().CreateRequest();
			httpReuestGetMaterials->SetVerb(TEXT("GET"));
			httpReuestGetMaterials->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
			httpReuestGetMaterials->SetURL(urlOfGetMaterials);
			httpReuestGetMaterials->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_GetMaterials);
			httpReuestGetMaterials->ProcessRequest();
		}
	}
}
