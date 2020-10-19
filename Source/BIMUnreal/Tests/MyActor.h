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

	// 可编程 Mesh 组件
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

	// 贴图名称的 Set
	// --------------
	TSet<FString> m_TextureNameSet;
	int m_TextureCount = 0;

	TMap<FString, FString> m_MapBlockMaterial;

	//TArray<FMaterial2> m_Materials;
	TMap<FString, FMaterial2> m_MapMaterial;

	// 材质执行分片
	// ------------
	//TQueue<FMaterialSection *> MaterialSectionQueue;
	TArray<FMaterialSection *> MaterialSectionArr;

	/*UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterialInstance * MaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterialInstanceDynamic * MaterialInstanceDnm;*/

	/* blockId 与 FProcMeshSection 的映射 */
	TMap<FString, FProcMeshSection> meshSectionMap;
	TMap<FString, UTexture2D*> m_TextureMap;
	TMap<FString, UMaterialInstanceDynamic*> dynamicMaterialMap;

	TArray<BYTE*> contentBufferFromCTMAnalysis;

	bool m_HasApperenanceColor;

	class UMaterialInterface* pTextureMaterial;
	class UMaterialInterface* pBaseColorMaterial;
	class UMaterialInterface* pTranslucentMaterial;

protected:

	// CTM 上下文句柄
	// --------------
	CTMcontext context;

	void setTextureFromLoadImg(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 请求回调
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

	// 解析 block buffer
	// -----------------
	void AnalysisBuffer(BYTE * buffer, int size);

	void SpawnActorsByElements(TArray<FElement> eles);

	// 将 TArray<FVector> 转为 FProcMeshSection对象内部需要的类型
	// ----------------------------------------------------------
	TArray<FProcMeshVertex> ConvertVertexToProcVertex(TArray<FVector> vertex, TArray<FVector2D> uvs);

	// 将 transform_list 转为 Spawn 中需要的参数
	// -----------------------------------------
	FTransform GetElementTransform(TArray<double> transform_list);

	// 根据 buffer 及名称生成 texture
	// ------------------------------
	void CreateTexture(TArray<uint8>* buffer, const FString textureName, EImageFormat defaultF = EImageFormat::JPEG);

	// 遍历所有视图，找到那个默认视图，并进行加载？
	// -------------------------------------------
	void ReadViewInfos(TArray<FViewInfo> viewInfos);

	// 创建材质的函数
	// --------------
	void CreateTextureMaterial(const FString materialId, FColor baseColor, FMaterialContent& content);
	void CreateBaseMaterial(const FString materialId, FColor baseColor, FMaterialContent& content);
	void CreateTranslucentMaterial(const FString materialId, FColor baseColor, FMaterialContent& content);
};
