// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "ProceduralMeshComponent.h" //ProceduralMeshComponent Module
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
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * ProceduralMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ����ص�
	// --------
	void OnRequestComplete_AllElementsInView(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestComplete_CacheBlockCount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestComplete_GetCacheBlock(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// ���� block buffer
	// -----------------
	void AnalysisBuffer(BYTE * buffer, int size);

};
