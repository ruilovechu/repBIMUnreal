// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstance.h"
#include "Materials/Material.h"
#include "ProceduralMeshComponent.h"

/**
 * ����ִ�з�Ƭ
 */
class BIMUNREAL_API FMaterialSection
{
public:

	UTexture2D * Texture2D;

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
};
