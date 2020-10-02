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
 * ����ִ�з�Ƭ
 */
class BIMUNREAL_API FMaterialSection
{
public:

	UTexture2D * Texture2D = 0;

	// ��ָ��Ķ�̬����ʵ��
	// --------------------
	UMaterialInstanceDynamic *	m_MaterialInstanceDyn;

	// ��Ƭ���
	// --------
	int							m_MaterialSectionIndex;
	TArray<FVector> verticesArr;
	TArray<FVector> normalsArr;
	TArray<int32> indicesArr;
	TArray<FVector2D> uvsArr;

	// ��ָ��� ProceduralMeshComp �������ͬһ�������У�����Ԫ�صĸ�ֵӦ��ָ��ͬһ������
	// -----------------------------------------------------------------------------------
	UProceduralMeshComponent *  m_ProceduralMeshComp;
	UMaterial* Material;
	FString m_blockId;
	FString m_MaterialId;



public:
	void setTextureFromLoadImg2(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful);
 };
