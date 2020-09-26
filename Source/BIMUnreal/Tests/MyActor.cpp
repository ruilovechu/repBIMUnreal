// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "../Common/OpenCTMHandler.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// openCTM dll “˝»Î≤‚ ‘
	// --------------------
	if (AOpenCTMHandler::m_CtmAddUVMap && AOpenCTMHandler::m_CtmCompressionLevel)
	{
		UE_LOG(LogTemp, Warning, TEXT("all are not null 1543"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("some is nulla"));
	}
}

//// Called every frame
//void AMyActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

