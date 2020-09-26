// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenCTMHandler.h"
#include "Misc/Paths.h"

#define POINTER(ctmName) (AOpenCTMHandler::GetFuncPointer(ctmName))

// 声明静态成员
// ------------
FString AOpenCTMHandler::m_OpenCTMDllPath = AOpenCTMHandler::GetDllPath();
void * AOpenCTMHandler::m_OpenCTMDllHandle = AOpenCTMHandler::GetCTMDllHandle();
FP_ctmNewContext AOpenCTMHandler::m_CtmNewContext = (FP_ctmNewContext)POINTER("ctmNewContext");
FP_ctmFreeContext AOpenCTMHandler::m_CtmFreeContext = (FP_ctmFreeContext)POINTER("ctmFreeContext");
FP_ctmGetError AOpenCTMHandler::m_CtmGetError = (FP_ctmGetError)POINTER("ctmGetError");
FP_ctmErrorString AOpenCTMHandler::m_CtmErrorString = (FP_ctmErrorString)POINTER("ctmErrorString");
FP_ctmGetInteger AOpenCTMHandler::m_CtmGetInteger = (FP_ctmGetInteger)POINTER("ctmGetInteger");
FP_ctmGetFloat AOpenCTMHandler::m_CtmGetFloat = (FP_ctmGetFloat)POINTER("ctmGetFloat");
FP_ctmGetIntegerArray AOpenCTMHandler::m_CtmGetIntegerArray = (FP_ctmGetIntegerArray)POINTER("ctmGetIntegerArray");
FP_ctmGetFloatArray AOpenCTMHandler::m_CtmGetFloatArray = (FP_ctmGetFloatArray)POINTER("ctmGetFloatArray");
FP_ctmGetNamedUVMap AOpenCTMHandler::m_CtmGetNamedUVMap = (FP_ctmGetNamedUVMap)POINTER("ctmGetNamedUVMap");
FP_ctmGetUVMapString AOpenCTMHandler::m_CtmGetUVMapString = (FP_ctmGetUVMapString)POINTER("ctmGetUVMapString");
FP_ctmGetUVMapFloat AOpenCTMHandler::m_CtmGetUVMapFloat = (FP_ctmGetUVMapFloat)POINTER("ctmGetUVMapFloat");
FP_ctmGetNamedAttribMap AOpenCTMHandler::m_CtmGetNamedAttribMap = (FP_ctmGetNamedAttribMap)POINTER("ctmGetNamedAttribMap");
FP_ctmGetAttribMapString AOpenCTMHandler::m_CtmGetAttribMapString = (FP_ctmGetAttribMapString)POINTER("ctmGetAttribMapString");
FP_ctmGetAttribMapFloat AOpenCTMHandler::m_CtmGetAttribMapFloat = (FP_ctmGetAttribMapFloat)POINTER("ctmGetAttribMapFloat");
FP_ctmGetString AOpenCTMHandler::m_CtmGetString = (FP_ctmGetString)POINTER("ctmGetString");
FP_ctmCompressionMethod AOpenCTMHandler::m_CtmCompressionMethod = (FP_ctmCompressionMethod)POINTER("ctmCompressionMethod");
FP_ctmCompressionLevel AOpenCTMHandler::m_CtmCompressionLevel = (FP_ctmCompressionLevel)POINTER("ctmCompressionLevel");
FP_ctmVertexPrecision AOpenCTMHandler::m_CtmVertexPrecision = (FP_ctmVertexPrecision)POINTER("ctmVertexPrecision");
FP_ctmVertexPrecisionRel AOpenCTMHandler::m_CtmVertexPrecisionRel = (FP_ctmVertexPrecisionRel)POINTER("ctmVertexPrecisionRel");
FP_ctmNormalPrecision AOpenCTMHandler::m_CtmNormalPrecision = (FP_ctmNormalPrecision)POINTER("ctmNormalPrecision");
FP_ctmUVCoordPrecision AOpenCTMHandler::m_CtmUVCoordPrecision = (FP_ctmUVCoordPrecision)POINTER("ctmUVCoordPrecision");
FP_ctmAttribPrecision AOpenCTMHandler::m_CtmAttribPrecision = (FP_ctmAttribPrecision)POINTER("ctmAttribPrecision");
FP_ctmFileComment AOpenCTMHandler::m_CtmFileComment = (FP_ctmFileComment)POINTER("ctmFileComment");
FP_ctmDefineMesh AOpenCTMHandler::m_CtmDefineMesh = (FP_ctmDefineMesh)POINTER("ctmDefineMesh");
FP_ctmAddUVMap AOpenCTMHandler::m_CtmAddUVMap = (FP_ctmAddUVMap)POINTER("ctmAddUVMap");
FP_ctmAddAttribMap AOpenCTMHandler::m_CtmAddAttribMap = (FP_ctmAddAttribMap)POINTER("ctmAddAttribMap");
FP_ctmLoad AOpenCTMHandler::m_CtmLoad = (FP_ctmLoad)POINTER("ctmLoad");
FP_ctmLoadCustom AOpenCTMHandler::m_CtmLoadCustom = (FP_ctmLoadCustom)POINTER("ctmLoadCustom");
FP_ctmSave AOpenCTMHandler::m_CtmSave = (FP_ctmSave)POINTER("ctmSave");
FP_ctmSaveToBuffer AOpenCTMHandler::m_CtmSaveToBuffer = (FP_ctmSaveToBuffer)POINTER("ctmSaveToBuffer");
FP_ctmFreeBuffer AOpenCTMHandler::m_CtmFreeBuffer = (FP_ctmFreeBuffer)POINTER("ctmFreeBuffer");
FP_ctmSaveCustom AOpenCTMHandler::m_CtmSaveCustom = (FP_ctmSaveCustom)POINTER("ctmSaveCustom");

// Sets default values
AOpenCTMHandler::AOpenCTMHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOpenCTMHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// 提供赋值给 m_OpenCTMDllPath 的方法
// ----------------------------------
FString AOpenCTMHandler::GetDllPath()
{
	UE_LOG(LogTemp, Warning, TEXT("AOpenCTMHandler::GetDllPath has been called!"));
	FString gamePath = FPaths::ConvertRelativePathToFull(FPaths::GameDir());
	FString dllpath = gamePath + "/ThirdParty/lib/openctm.dll";
	return dllpath;
}

// 提供赋值给 m_OpenCTMDllHandle 的方法
// ------------------------------------
void * AOpenCTMHandler::GetCTMDllHandle()
{
	FString dllPath = AOpenCTMHandler::m_OpenCTMDllPath;
	void * openctmDllHandle = FPlatformProcess::GetDllHandle(*dllPath);
	return openctmDllHandle;
}


void * AOpenCTMHandler::GetFuncPointer(FString funName)
{
	void * pFunc = FPlatformProcess::GetDllExport(m_OpenCTMDllHandle, *funName);
	return pFunc;
}

//FP_ctmFreeContext AOpenCTMHandler::GetCtmFreeContext(FString funName)
//{
//	FP_ctmFreeContext pFunc = (FP_ctmFreeContext)FPlatformProcess::GetDllExport(m_OpenCTMDllHandle, *funName);
//	return pFunc;
//}

//// Called every frame
//void AOpenCTMHandler::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

