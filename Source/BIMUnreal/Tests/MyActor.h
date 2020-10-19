// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "ProceduralMeshComponent.h" //ProceduralMeshComponent Module
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"
#include "Containers/Queue.h"
#include "../Model/FMaterialSection.h"
#include "../Model/FMaterial.h"
#include "../Common/OpenCTMHandler.h"
#include "../Model/FElement.h"
#include "../Model/FViewInfo.h"
#include "MyActor.generated.h"

UCLASS()
class BIMUNREAL_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	// �ɱ�� Mesh ���
	// ----------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent * ProceduralMeshComp;

	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterial* Material;

	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UTexture2D * Texture2D;

	// meshIndex
	// ---------
	int m_meshIndex;

	int m_blockFileCount;
	int m_blockFileRead = 0;

	// ��ͼ���Ƶ� Set
	// --------------
	TSet<FString> m_TextureNameSet;
	int m_TextureCount = 0;

	TMap<FString, FString> m_MapBlockMaterial;

	//TArray<FMaterial2> m_Materials;
	TMap<FString, FMaterial2> m_MapMaterial;

	// ����ִ�з�Ƭ
	// ------------
	//TQueue<FMaterialSection *> MaterialSectionQueue;
	TArray<FMaterialSection *> MaterialSectionArr;

	/*UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterialInstance * MaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterialInstanceDynamic * MaterialInstanceDnm;*/

	/* blockId �� FProcMeshSection ��ӳ�� */
	TMap<FString, FProcMeshSection> meshSectionMap;
	TMap<FString, UTexture2D*> m_TextureMap;
	TMap<FString, UMaterialInstanceDynamic*> dynamicMaterialMap;

	TArray<BYTE*> contentBufferFromCTMAnalysis;

	bool m_HasApperenanceColor;

	class UMaterialInterface* pTextureMaterial;
	class UMaterialInterface* pBaseColorMaterial;
	class UMaterialInterface* pTranslucentMaterial;

protected:

	// CTM �����ľ��
	// --------------
	CTMcontext context;

	void setTextureFromLoadImg(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ����ص�
	// --------
	void OnRequestComplete_AllElementsInView(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestComplete_CacheBlockCount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestComplete_GetCacheBlock(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestComplete_GetMaterials(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestComplete_GetModel(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestComplete_GetTexture(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ���� block buffer
	// -----------------
	void AnalysisBuffer(BYTE * buffer, int size);

	void SpawnActorsByElements(TArray<FElement> eles);

	// �� TArray<FVector> תΪ FProcMeshSection�����ڲ���Ҫ������
	// ----------------------------------------------------------
	TArray<FProcMeshVertex> ConvertVertexToProcVertex(TArray<FVector> vertex, TArray<FVector2D> uvs);

	// �� transform_list תΪ Spawn ����Ҫ�Ĳ���
	// -----------------------------------------
	FTransform GetElementTransform(TArray<double> transform_list);

	// ���� buffer ���������� texture
	// ------------------------------
	void CreateTexture(TArray<uint8>* buffer, const FString textureName, EImageFormat defaultF = EImageFormat::JPEG);

	// ����������ͼ���ҵ��Ǹ�Ĭ����ͼ�������м��أ�
	// -------------------------------------------
	void ReadViewInfos(TArray<FViewInfo> viewInfos);

	// �������ʵĺ���
	// --------------
	void CreateTextureMaterial(const FString materialId, FColor baseColor, FMaterialContent& content);
	void CreateBaseMaterial(const FString materialId, FColor baseColor, FMaterialContent& content);
	void CreateTranslucentMaterial(const FString materialId, FColor baseColor, FMaterialContent& content);
};
