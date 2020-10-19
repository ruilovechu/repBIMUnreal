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

// 路径转为 UTexture2D
// -------------------
void AMyActor::setTextureFromLoadImg(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful)
{
	if (!bWasSuccessful && !_response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT(" !bWasSuccessful && !_response.IsValid() "));
		return;
	}

	IImageWrapperModule& temp_img_module = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	IImageWrapperPtr temp_imgWrapper = temp_img_module.CreateImageWrapper(EImageFormat::JPEG);//JPG 或 png 等

	TArray<uint8> temp_fileData = _response->GetContent();


	if (!temp_imgWrapper.IsValid() ||
		!temp_imgWrapper->SetCompressed(temp_fileData.GetData(), temp_fileData.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ImageWrapper can‘t Set Compressed or ImageWrapper is InValid"));
		return;
	}

	const TArray<uint8>* temp_unCompressedRGBA = NULL;

	if (!temp_imgWrapper->GetRaw(ERGBFormat::RGBA, 8, temp_unCompressedRGBA))
	{
		UE_LOG(LogTemp, Warning, TEXT("can‘t get Raw temp_unCompressedRGBA"));
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

	//// 请求接口测试2
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

	//必须放到构造函数中
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
	//创建半透明材质
	auto pMaterialInstance = UMaterialInstanceDynamic::Create(pTranslucentMaterial, this, FName(*materialId));

	//设置基础颜色
	pMaterialInstance->SetVectorParameterValue("MainColor", baseColor);
	//设置透明度
	pMaterialInstance->SetScalarParameterValue("Opacity", content.tp);
	//设置粗糙度
	pMaterialInstance->SetScalarParameterValue("Roughness", 1 - content.Glossness);

	dynamicMaterialMap.Emplace(materialId, pMaterialInstance);
}

void AMyActor::CreateBaseMaterial(const FString materialId, FColor baseColor, FMaterialContent& content)
{
	//创建基础材质
	auto pMaterialInstance = UMaterialInstanceDynamic::Create(pBaseColorMaterial, this, FName(*materialId));

	//设置基础颜色
	pMaterialInstance->SetVectorParameterValue("MainColor", baseColor);

	//设置材质的金属度
	float metallicValue = content.ismetal == true ? 1.0f : 0.0f;
	pMaterialInstance->SetScalarParameterValue("Metallic", metallicValue);

	//设置粗糙度
	pMaterialInstance->SetScalarParameterValue("Roughness", 1 - content.Glossness);

	dynamicMaterialMap.Emplace(materialId, pMaterialInstance);
}

void AMyActor::CreateTextureMaterial(const FString materialId, FColor baseColor, FMaterialContent& content)
{

	//创建纹理材质
	auto pMaterialInstance = UMaterialInstanceDynamic::Create(pTextureMaterial, this, FName(*materialId));
	pMaterialInstance->SetTextureParameterValue("MainTexture", m_TextureMap[content.img]);

	//设置纹理的UV偏移
	pMaterialInstance->SetScalarParameterValue("OffsetX", content.uf);
	pMaterialInstance->SetScalarParameterValue("OffsetY", content.vf);

	//设置纹理的UV缩放
	content.usc != 0.0f ? content.usc : 1.0f;
	content.vsc != 0.0f ? content.vsc : 1.0f;

	pMaterialInstance->SetScalarParameterValue("ScaleX", content.usc);
	pMaterialInstance->SetScalarParameterValue("ScaleY", content.vsc);

	//设置基础颜色
	pMaterialInstance->SetVectorParameterValue("MainColor", baseColor);

	//设置材质的金属度
	float metallicValue = content.ismetal == true ? 1.0f : 0.0f;
	pMaterialInstance->SetScalarParameterValue("Metallic", metallicValue);

	//设置粗糙度
	pMaterialInstance->SetScalarParameterValue("Roughness", 1 - content.Glossness);

	dynamicMaterialMap.Emplace(materialId, pMaterialInstance);
}


// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// 初始化 CTM 的 content 句柄
	// [TODO]暂未释放
	// --------------
	this->context = AOpenCTMHandler::m_CtmNewContext(CTMenum::CTM_IMPORT);

	// 清空 blockId 与 materialId 的 map
	// ---------------------------------
	m_MapBlockMaterial.Empty();

	// 清空 m_MapMaterial 数据
	// -----------------------
	m_MapMaterial.Empty();

	// 清空 m_TextureNameSet 数据
	// --------------------------
	m_TextureNameSet.Empty();
	m_TextureMap.Empty();

	// 清掉所有的 MeshSection !
	// ------------------------
	ProceduralMeshComp->ClearAllMeshSections();

	// 地址拼接测试
	// ------------
	// https://bimcomposer.probim.cn/api/Model/GetAllElementsInView?ProjectID=7d96928d-5add-45cb-a139-2c787141e50d&ModelID=9f49078c-180e-4dc5-b696-5a50a9e09016&VersionNO=&ViewID=142554
	//AUrlsHandler::InitParameters(FString("46d11566-6b7e-47a1-ba5d-12761ab9b55c"), FString("67170069-1711-4f4c-8ee0-a715325942a1"), FString("69323"));
	AUrlsHandler::InitParameters(FString("46d11566-6b7e-47a1-ba5d-12761ab9b55c"), FString("726f5ad8-0123-459f-8566-475146d396ef"), FString("142554"));
	
	// 请求接口测试1
	// 调用GetModel
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
		// 将返回的json进行反序列化
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

			// 遍历所有视图，找到那个默认视图，并进行加载？
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

			// 打印输出 eles 的个数
			// --------------------
			UE_LOG(LogTemp, Warning, TEXT("eles' count is %d"), eles.Num());
			//UE_LOG(LogTemp, Error, TEXT("7 ---- %d"), (eles[eles.Num() - 1].BlockMapping_obj.blockMappingItems_objlist[0].transform_list.Num()));

			// 根据这些 eles，来进行 Spawn Actor
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
		// 拿到 res，得到接口返回的字符串
		// ------------------------------
		IHttpResponse * res = Response.Get();
		FString ContentJson = Response->GetContentAsString();

		// 将返回的字符串转为数字，作为获得的 block 的个数
		// -----------------------------------------------
		int blockFileCount = FCString::Atoi(*ContentJson);
		this->m_blockFileCount = blockFileCount;

		// 清一下 meshSectionMap
		// ---------------------
		meshSectionMap.Empty();

#if 0
		// 先调用 GetMaterials
		// -------------------
		FString urlOfMaterials = AUrlsHandler::GetUrlOfGetMaterials();
		TSharedRef<IHttpRequest> httpReuestMtr = FHttpModule::Get().CreateRequest();
		httpReuestMtr->SetVerb(TEXT("GET"));
		httpReuestMtr->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
		httpReuestMtr->SetURL(urlOfMaterials);
		httpReuestMtr->OnProcessRequestComplete().BindUObject(this, &AMyActor::OnRequestComplete_GetMaterials);
		httpReuestMtr->ProcessRequest();
#endif

		// 从1开始，针对每个数字，再调用 https://bimcomposer.probim.cn/api/Model/GetCacheBlock?FileID=1&ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=58080653-18d1-4067-b480-e02c56eb791a&VersionNO=&ViewID=168550
		// -------------------------------------------------------------------------------------------
		for (int i = 1; i <= m_blockFileCount; i++)
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
		// 里面构造了 meshSectionMap
		// --------------------------
		AMyActor::AnalysisBuffer(buffer, resBuffer.Num());

		this->m_blockFileRead++;

		// 判断如果调用完最后一次的block file文件，则开始调用GetAllElementsInView
		// ----------------------------------------------------------------------
		if (this->m_blockFileRead == this->m_blockFileCount)
		{
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

		//从URL中提取纹理的名称
		// --------------------
		FString fileName = Request->GetURLParameter("FileName");

		// 根据 buffer 生成 Texture，并添加到 TextureMap
		// ---------------------------------------------
		CreateTexture(&imageData, fileName);
		this->m_TextureCount++;

		// 全部纹理生成完成后，创建材质
		// ----------------------------
		if (this->m_TextureNameSet.Num() == m_TextureCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("Begin to Create Material"));

			// 创建材质并添加到 dynamicMaterialMap
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

				//判断是否是半透明材质
				if (materialContent.tp < 1.0f)
				{
					//创建半透明材质
					CreateTranslucentMaterial(material.Key, color, materialContent);
				}
				else
				{
					if (!imgName.IsEmpty() && this->m_TextureMap.Contains(imgName))
					{
						//创建纹理材质
						CreateTextureMaterial(material.Key, color, materialContent);
					}
					else
					{
						//创建基础颜色材质
						CreateBaseMaterial(material.Key, color, materialContent);
					}
				}

			}
		
			// 请求 blockCount 等后续操作
			// --------------------------
			// 请求接口测试2
			// 后续放到 GetModel的 回调中
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

	//创建Texture2D纹理
	auto pTexture = UTexture2D::CreateTransient(pImageWrapper->GetWidth(), pImageWrapper->GetHeight(), PF_B8G8R8A8, FName(*textureName));

	//锁住纹理数据，以便修改
	auto pTextureData = static_cast<uint8*>(pTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	FMemory::Memcpy(pTextureData, unCompressedRGBA->GetData(), unCompressedRGBA->Num());

	//解锁纹理
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

		// 将 allMaterials 反序列化为实体
		// ------------------------------
		TArray<TSharedPtr<FJsonValue>> originParsed;
		bool BFlag = AJsonHandler::DeserializeList(allMaterials, &originParsed);
		if (BFlag)
		{
			//// 贴图名称的 Set
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
				// 添加材质id 与 内容的 map
				// ------------------------
				m_MapMaterial.Add(mtr.ID, mtr);

				// 如果有贴图，添加名称到 Set 中
				// -----------------------------
				if (!mtr.Content_Obj.img.IsEmpty())
				{
					m_TextureNameSet.Add(mtr.Content_Obj.img);
				}
			}

			// 遍历 m_TextureNameSet，来下载贴图
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

	//	// 将 贴图赋予该 材质
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

	//	// 将材质赋予该 mesh 分片
	//	// ----------------------
	//	section->m_ProceduralMeshComp->SetMaterial(section->m_MaterialSectionIndex, section->m_MaterialInstanceDyn);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Dequeue no data!"));
	//}
}

// ctmLoadCustom 回调
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


// 解析这个字节数组
// ----------------
void AMyActor::AnalysisBuffer(BYTE * buffer, int size)
{
	int offset = 0;

	int blockloaded = 0;

	// 由于返回的数据可能不止含有一段 |B365|
	// 这里用循环来取多个 blockBuffer
	// ------------------------------
	while (offset < size)
	{
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

		// 声明 blockIdLength 个字节的 BYTE，用于存储 blockId 的值
		// -------------------------------------------------------
		BYTE * blockIdBuffer = (BYTE*)calloc(blockIdLength, sizeof(BYTE));

		// 拷贝 blockIdLength 个字节到 blockIdBuffer
		// 取完后马上记录偏移位置
		// ----------------------
		memcpy(blockIdBuffer, buffer + offset, blockIdLength);
		TCHAR* blockIdBuffer_utf8 = UTF8_TO_TCHAR(blockIdBuffer);
		offset += blockIdLength;

		// 接下来应该是 |ID GEO| 这个字符串，拷贝输出验证
		// ----------------------------------------------
		TCHAR ch[9] = { 0 };
		memcpy(ch, buffer + offset, 8);
		TCHAR * ch_uft8 = UTF8_TO_TCHAR(ch);
		offset += strlen("|ID GEO|");

		// 再读4个字节，拿到contentLength
		// ------------------------------
		int contentLength;
		memcpy(&contentLength, buffer + offset, sizeof(int));
		offset += 4;

		// 先读 contentLength 个字节，读完后再偏移contentLength 个字节，进入下一个循环
		// ---------------------------------------------------------------------------
		BYTE * contentBuffer = (BYTE*)calloc(contentLength, sizeof(BYTE));
		memcpy(contentBuffer, buffer + offset, contentLength);
		offset += contentLength;
		
		if (this->context)
		{
			// 调用 openCTM 的 LoadCustom
			// --------------------------
			AOpenCTMHandler::m_CtmLoadCustom(this->context, CTMRreadfn, contentBuffer);

			// 调用各 api 获取数据
			// -------------------
			int vertex_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(this->context, CTMenum::CTM_VERTEX_COUNT) * VERTEX_3;
			int indicate_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(this->context, CTMenum::CTM_TRIANGLE_COUNT) * INDICATE_3; // 索引个数
			int uv_comp_cnt = AOpenCTMHandler::m_CtmGetInteger(this->context, CTMenum::CTM_VERTEX_COUNT) * VERTEX_UV_2;

			// 获取数据
			// 顶点数组
			// 法线数组
			// 索引数组
			// uv数组
			// ------
			TArray<FVector> verticesArr;
			TArray<FVector> normalsArr;
			TArray<uint32> indicesArr;
			TArray<FVector2D> uvsArr;
			//TArray<FLinearColor> linearArr;

			// 获取顶点数组
			// ------------
			const CTMfloat * vertices = AOpenCTMHandler::m_CtmGetFloatArray(this->context, CTMenum::CTM_VERTICES);
			for (size_t i = 0; i < vertex_comp_cnt; i += VERTEX_3)
			{
				//UE4中是模型是以厘米为单位
				//verticesArr.Add(FVector(vertices[i] * 10, vertices[i + 1] * 10, vertices[i + 2] * 10));
				//linearArr.Add(FLinearColor(255.0f, 0, 0, 0.8));
				// ----------------------------------------------
				verticesArr.Add(FVector(vertices[i] * 100, vertices[i + 1] * -100, vertices[i + 2] * 100));
			}

			// 获取索引数组
			// ------------
			const CTMuint * indicates = AOpenCTMHandler::m_CtmGetIntegerArray(this->context, CTMenum::CTM_INDICES);
			for (size_t i = 0; i < indicate_comp_cnt; i++)
			{
				indicesArr.Add(indicates[i]);
			}

			// 获取uv数组
			// ----------
			const CTMfloat * uvs = AOpenCTMHandler::m_CtmGetFloatArray(this->context, CTMenum::CTM_UV_MAP_1);
			if (uvs)
			{
				for (size_t i = 0; i < uv_comp_cnt; i += VERTEX_UV_2)
				{
					// 以厘米为单位
					// ------------
					uvsArr.Add(FVector2D(uvs[i], uvs[i + 1]));
					//uvsArr.Add(FVector2D(uvs[i] * 1, uvs[i + 1] * 1));
				}
			}

			// *获取法线数组(vertex_comp_cnt 与顶点（分量）个数相同)
			// 这部分可以不填，但会影响光照。当你必须要计算反光效果时候，这些参数必须计算。顶点的法线一般可以理解为：在该点所在的三角面上由该点与另外的两个点组成的两个向量的叉乘（一般向量方向由右手法则来确定），叉乘结果再归一化。
			// 法线数组，数组的大小等于网格顶点坐标的数量
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
			// 构造 FProcMeshSection
			// 一个blockFile中的内容可能包含一个或多个block的相关数据
			// 每一个blockId对应一个meshSection
			// --------------------------------
			FProcMeshSection meshSection;
			/* 三角形顶点索引 */
			meshSection.ProcIndexBuffer = indicesArr;
			/* 顶点数据 */
			meshSection.ProcVertexBuffer = ConvertVertexToProcVertex(verticesArr, uvsArr);
			/* bSectionVisible 及 bEnableCollision */
			meshSection.bSectionVisible = true;
			meshSection.bEnableCollision = false;

			// 添加到 meshSectionMap
			// ---------------------
			FString blockId = FString(blockIdBuffer_utf8);
			meshSectionMap.Emplace(blockId, meshSection);

			//// 添加到数组
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

			// 直接卸载
			// --------
			//AOpenCTMHandler::m_CtmFreeContext(context);
			//context = NULL;
		}

		// 写完 calloc，马上写 free
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
	// 这里测试：一共有多少个 Mesh 分片
// --------------------------------
	int sectionNum = MaterialSectionArr.Num(); //ProceduralMeshComp->GetNumSections();
	UE_LOG(LogTemp, Warning, TEXT("[721:7]MaterialSectionArr's Num is %d"), sectionNum);

	// test
	// ----
	int mapnum = m_MapBlockMaterial.Num();
	UE_LOG(LogTemp, Warning, TEXT("map's num is %d"), mapnum);

	// 测试，遍历并赋予 Texture2D
	// --------------------------
	for (size_t i = 0; i < sectionNum; i++)
	{
		//// 从 m_MapMaterial 找到 materialId 对应的数据
		//// -----------------------------------------
		//auto materialId = this->m_MapBlockMaterial[MaterialSectionArr[i]->m_blockId];

		//FMaterial2 & mtrObj = m_MapMaterial[materialId];
		//UE_LOG(LogTemp, Warning, TEXT("[759]color is %s"), *(mtrObj.Content_Obj.appearancecolor));

		// 创建Actor
		// ---------
		//UClass* pBlueprintClass = StaticLoadClass(AElementActor::StaticClass(), nullptr, TEXT("Blueprint'/Game/Blueprints/Tests/BP_ElementActor.BP_ElementActor_C'"));
		UClass* pBlueprintClass = StaticLoadClass(AElementActor::StaticClass(), this, TEXT("Blueprint'/Game/Blueprints/Tests/BP_ElementActor.BP_ElementActor_C'"));

		if (pBlueprintClass)
		{
			// 动态添加一个 Actor
			// ------------------
			AElementActor * theActor = GetWorld()->SpawnActor<AElementActor>(pBlueprintClass);

			// 为这个刚添加的 Actor 创建 Mesh 分片
			// 每个Actor只负责一个分片
			// -----------------------
			theActor->ProceduralMeshComp->CreateMeshSection_LinearColor(0, MaterialSectionArr[i]->verticesArr, MaterialSectionArr[i]->indicesArr, MaterialSectionArr[i]->normalsArr, MaterialSectionArr[i]->uvsArr, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);

			//// 创建动态材质实例
			//// ----------------
			//auto dynMaterial = UMaterialInstanceDynamic::Create(theActor->Material, theActor, FName(*(FString("Name_") + FString::FromInt(0))));
			//theActor->m_MaterialInstanceDyn = dynMaterial;

			//// 修改这个材质的一个参数
			//// ----------------------
			//dynMaterial->SetVectorParameterValue("AV3", FLinearColor(0.4f, 0.4f, 0.4f, 1));

			//// 设置到这个 actor 的 Mesh 上
			//// ---------------------------
			//theActor->ProceduralMeshComp->SetMaterial(0, dynMaterial);

			//// 调用接口，获取贴图
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
		//// CreateMeshSection_LinearColor 推荐使用此方法
		//// --------------------------------------------
		//MaterialSectionArr[i]->m_ProceduralMeshComp->CreateMeshSection_LinearColor(i, MaterialSectionArr[i]->verticesArr, MaterialSectionArr[i]->indicesArr, MaterialSectionArr[i]->normalsArr, MaterialSectionArr[i]->uvsArr, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);

		//// 指向材质
		//// --------
		//MaterialSectionArr[i]->m_MaterialInstanceDyn = UMaterialInstanceDynamic::Create(this->Material, this, FName(*(FString("MI_Dynamic") + FString::FromInt(i))));

		//// 设置材质参数 颜色
		//// -----------------
		///*	MaterialSectionArr[i]->m_MaterialInstanceDyn->SetVectorParameterValue("AV3", FLinearColor(100.0f, 0,0, 0.2));*/

		//// 调用接口，获取贴图
		//// ------------------
		//if (mtrObj.Content_Obj.img != FString(""))
		//{
		//	TSharedRef<IHttpRequest> httpReuestimg = FHttpModule::Get().CreateRequest();
		//	httpReuestimg->SetVerb(TEXT("GET"));
		//	httpReuestimg->SetHeader(TEXT("Content-Type"), TEXT("APPLICATION/x-www-from-urlencoded"));
		//	httpReuestimg->SetURL(AUrlsHandler::GetUrlOfGetTextureFile(mtrObj.Content_Obj.img));
		//	httpReuestimg->OnProcessRequestComplete().BindRaw(MaterialSectionArr[i], &FMaterialSection::setTextureFromLoadImg2);
		//	httpReuestimg->ProcessRequest();

		//	// 设置材质参数 贴图
		//	// -----------------
		//	//if (tmpTexture)//MaterialSectionArr[i]->Texture2D)
		//	//{
		//	//	MaterialSectionArr[i]->m_MaterialInstanceDyn->SetTextureParameterValue("TV5", tmpTexture);// MaterialSectionArr[i]->Texture2D);
		//	//}
		//}

		//// 设置材质到 Mesh 组件
		//// --------------------
		//MaterialSectionArr[i]->m_ProceduralMeshComp->SetMaterial(MaterialSectionArr[i]->m_MaterialSectionIndex, MaterialSectionArr[i]->m_MaterialInstanceDyn);

	}
#endif

	//// 测试
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
			// 拿到这个element 中的 blockMappingItems
			// --------------------------------------
			TArray<FBlockMapping_Item> items = eles[i].BlockMapping_obj.blockMappingItems_objlist;

			// 遍历这些 items
			// --------------
			for (size_t j = 0; j < items.Num(); j++)
			{
				// 声明一个 Spawn 参数
				// -------------------
				FActorSpawnParameters SpawnPara;
				SpawnPara.Name = FName(*(eles[i].ElementID + TEXT("_") + FString::FromInt(j)));

				// 通过 items[j].transform_list 得到 Spawn 中用到的矩阵参数
				// --------------------------------------------------------
				FTransform actorTransform = GetElementTransform(items[j].transform_list);

				// 进行 Spawn
				// ----------
				AElementActor* pElementActor = GetWorld()->SpawnActor<AElementActor>(pBlueprintClass, actorTransform, SpawnPara);

				// 调用 loadMesh
				// -------------
			/*	FProcMeshSection* mesh = meshSectionMap.Find(items[j].blockId);
				pElementActor->ProceduralMeshComp->SetProcMeshSection(0, *mesh);
				pElementActor->ProceduralMeshComp->UpdateBounds();*/

#if 1 // 未复用 Mesh 版

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

// 将 TArray<FVector> 转为 FProcMeshSection对象内部需要的类型
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

	//UE中的坐标单位为厘米，而传入数据的坐标单位为米
	mat.M[3][0] *= 100;
	mat.M[3][1] *= -100;
	mat.M[3][2] *= 100;

	//mat.M[0][3] *= 100;
	//mat.M[1][3] *= 100;
	//mat.M[2][3] *= 100;

	return FTransform(mat);
}

// 遍历所有视图，找到那个默认视图，并进行加载？
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

			// 加载材质，内部用到 m_HasApperenanceColor
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
