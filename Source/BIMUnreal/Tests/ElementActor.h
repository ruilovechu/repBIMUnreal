// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h" //ProceduralMeshComponent Module

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Materials/MaterialInstance.h"

#include "ElementActor.generated.h"

UCLASS()
class BIMUNREAL_API AElementActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElementActor();


	// 可编程 Mesh 组件
	// ----------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent * ProceduralMeshComp;

	// 用于创建动态材质实例的基本对象
	// ------------------------------
	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterial* Material;

	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UTexture2D * Texture2D;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	FString ElementID;

	// 所指向的动态材质实例
	// --------------------
	UMaterialInstanceDynamic *	m_MaterialInstanceDyn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void setTextureFromLoadImg3(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful);


};
