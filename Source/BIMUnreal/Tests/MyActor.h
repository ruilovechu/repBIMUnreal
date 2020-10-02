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
#include "../Common/OpenCTMHandler.h"
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
	int m_meshIndex;

	// 材质执行分片
	// ------------
	//TQueue<FMaterialSection *> MaterialSectionQueue;
	TArray<FMaterialSection *> MaterialSectionArr;

	/*UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterialInstance * MaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterialInstanceDynamic * MaterialInstanceDnm;*/

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 解析 block buffer
	// -----------------
	void AnalysisBuffer(BYTE * buffer, int size);

};
