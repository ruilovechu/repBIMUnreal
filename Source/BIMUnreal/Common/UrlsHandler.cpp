// Fill out your copyright notice in the Description page of Project Settings.


#include "UrlsHandler.h"
#include "../Config/ConfigHandler.h"

FString AUrlsHandler::m_ProjectID = "";
FString AUrlsHandler::m_ModelID = "";
FString AUrlsHandler::m_ViewID = "";

// Sets default values
AUrlsHandler::AUrlsHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUrlsHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

int AUrlsHandler::InitParameters(FString projectID, FString modelID, FString viewID)
{
	AUrlsHandler::m_ProjectID = projectID;
	AUrlsHandler::m_ModelID = modelID;
	AUrlsHandler::m_ViewID = viewID;
	return 0;
}

FString AUrlsHandler::GetUrlOfGetAllElements()
{
	// https://bimcomposer.probim.cn/api/Model/GetAllElementsInView?ProjectID=7d96928d-5add-45cb-a139-2c787141e50d&ModelID=9f49078c-180e-4dc5-b696-5a50a9e09016&VersionNO=&ViewID=142554
	FString ret = AConfigHandler::GetModelApiUrl() + FString("/api/Model/GetAllElementsInView?ProjectID=") + m_ProjectID + FString("&ModelID=") + m_ModelID + FString("&VersionNO=&ViewID=") + m_ViewID;
	return ret;
}

FString AUrlsHandler::GetUrlOfGetBlockCacheCount()
{
	// https://bimcomposer.probim.cn/api/Model/GetBlockCacheCount?ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=58080653-18d1-4067-b480-e02c56eb791a&VersionNO=&ViewID=168550
	FString ret = AConfigHandler::GetModelApiUrl() + FString("/api/Model/GetBlockCacheCount?ProjectID=") + m_ProjectID + FString("&ModelID=") + m_ModelID + FString("&VersionNO=&ViewID=") + m_ViewID;
	return ret;
}

FString AUrlsHandler::GetUrlOfGetCacheBlock(int FileID)
{
	// https://bimcomposer.probim.cn/api/Model/GetCacheBlock?FileID=1&ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=58080653-18d1-4067-b480-e02c56eb791a&VersionNO=&ViewID=168550
	FString ret = AConfigHandler::GetModelApiUrl() + FString("/api/Model/GetCacheBlock?FileID=") + FString::FromInt(FileID) + FString("&ProjectID=") + m_ProjectID + FString("&ModelID=") + m_ModelID + FString("&VersionNO=&ViewID=") + m_ViewID;
	return ret;
}

FString AUrlsHandler::GetUrlOfGetMaterials()
{
	FString ret = AConfigHandler::GetModelApiUrl() + FString("/api/Model/GetMaterials?ProjectID=") + m_ProjectID + FString("&ModelID=") + m_ModelID + FString("&VersionNO=");
	return ret;
}

FString AUrlsHandler::GetUrlOfGetTextureFile(FString filenameziped)
{
	// https://bimcomposer.probim.cn/api/Model/GetFile?ProjectID=46d11566-6b7e-47a1-ba5d-12761ab9b55c&ModelID=67170069-1711-4f4c-8ee0-a715325942a1&VersionNO=&FileType=Texture&FileName=brickldnonlduniformldrunningldredppng
	FString ret = AConfigHandler::GetModelApiUrl() + FString("/api/Model/GetFile?ProjectID=") + m_ProjectID + FString("&ModelID=") + m_ModelID + FString("&VersionNO=&FileType=Texture&FileName=") + filenameziped;
	return ret;
}
