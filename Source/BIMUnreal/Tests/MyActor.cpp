// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Misc/Paths.h"

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
	FString gamePath = FPaths::ConvertRelativePathToFull(FPaths::GameDir());
	FString dllpath = gamePath + "/ThirdParty/lib/openctm.dll";
	void * openctmDllHandle = FPlatformProcess::GetDllHandle(*dllpath);
	if (openctmDllHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("openctmDllHandle is not null"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("openctmDllHandle is null"));
	}
}

//// Called every frame
//void AMyActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

