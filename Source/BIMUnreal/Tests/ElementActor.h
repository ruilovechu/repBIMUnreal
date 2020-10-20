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


	// �ɱ�� Mesh ���
	// ----------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent * ProceduralMeshComp;

	// ���ڴ�����̬����ʵ���Ļ�������
	// ------------------------------
	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UMaterial* Material;

	UPROPERTY(VisibleAnywhere, Category = ProcMesh)
	UTexture2D * Texture2D;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	FString ElementID;

	// ��ָ��Ķ�̬����ʵ��
	// --------------------
	UMaterialInstanceDynamic *	m_MaterialInstanceDyn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void setTextureFromLoadImg3(FHttpRequestPtr _request, FHttpResponsePtr _response, bool bWasSuccessful);


};
