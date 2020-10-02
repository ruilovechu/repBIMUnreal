// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstance.h"
#include "Materials/Material.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapperModule.h"
#include "ProceduralMeshComponent.h"

/**
 * 材质执行分片
 */
class BIMUNREAL_API FMaterialSection
{
public:

	UTexture2D * Texture2D = 0;

	// 所指向的动态材质实例
	// --------------------
	UMaterialInstanceDynamic *	m_MaterialInstanceDyn;

	// 分片编号
	// --------
	int							m_MaterialSectionIndex;
	TArray<FVector> verticesArr;
	TArray<FVector> normalsArr;
	TArray<int32> indicesArr;
	TArray<FVector2D> uvsArr;

	// 所指向的 ProceduralMeshComp 组件（在同一个队列中，所有元素的该值应该指向同一个对象）
	// -----------------------------------------------------------------------------------
	UProceduralMeshComponent *  m_ProceduralMeshComp;
	UMaterial* Material;
	FString m_blockId;
	FString m_MaterialId;



public:
	void setTextureFromLoadImg2(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful);
 };
