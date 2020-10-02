// Fill out your copyright notice in the Description page of Project Settings.


#include "FMaterialSection.h"

void FMaterialSection::setTextureFromLoadImg2(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful)
{
	if (!bWasSuccessful && !_response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT(" !bWasSuccessful && !_response.IsValid() "));
		return;
	}

	// 判断请求地址以什么来结尾
	// ------------------------
	EImageFormat format;
	FString URL = _request.Get()->GetURL();
	if (URL.EndsWith("png"))
	{
		format = EImageFormat::PNG;
	}
	else
	{
		format = EImageFormat::JPEG;
	}

	IImageWrapperModule& temp_img_module = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	IImageWrapperPtr temp_imgWrapper = temp_img_module.CreateImageWrapper(EImageFormat::JPEG);//JPG 或 png 等
	TArray<uint8> temp_fileData = _response->GetContent();

	if (!temp_imgWrapper.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ImageWrapper is not valid %s"), *URL);
		return;
	}

	if (!temp_imgWrapper->SetCompressed(temp_fileData.GetData(), temp_fileData.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ImageWrapper can‘t Set Compressed, url is %s"), *URL);
		return;
	}

	const TArray<uint8>* temp_unCompressedRGBA = NULL;

	if (!temp_imgWrapper->GetRaw(ERGBFormat::RGBA, 8, temp_unCompressedRGBA))
	{
		UE_LOG(LogTemp, Warning, TEXT("can‘t get Raw temp_unCompressedRGBA"));
		return;
	}

	this->Texture2D = UTexture2D::CreateTransient(temp_imgWrapper->GetWidth(), temp_imgWrapper->GetHeight());

	auto temp_dataPtr = static_cast<uint8*>(this->Texture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

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

	this->Texture2D->PlatformData->Mips[0].BulkData.Unlock();

	//Texture2D->UpdateResourceW();
	this->Texture2D->UpdateResource();

	// 设置材质参数 贴图
	// -----------------
	this->m_MaterialInstanceDyn->SetTextureParameterValue("TV5", Texture2D);

	//// 设置材质到 Mesh 组件
	//// --------------------
	//this->m_ProceduralMeshComp->SetMaterial(this->m_MaterialSectionIndex, this->m_MaterialInstanceDyn);

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