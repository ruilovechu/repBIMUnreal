// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../ThirdParty/include/OpenCTM/openctm.h"
#include "../../ThirdParty/include/OpenCTM/openctmpp.h"

// 类型定义
// --------
typedef CTMcontext(*FP_ctmNewContext)(CTMenum aMode);
typedef void(*FP_ctmFreeContext)(CTMcontext aContext);
typedef CTMenum (*FP_ctmGetError)(CTMcontext aContext);
typedef const char * (*FP_ctmErrorString)(CTMenum aError);
typedef CTMuint (*FP_ctmGetInteger)(CTMcontext aContext, CTMenum aProperty);
typedef CTMfloat(*FP_ctmGetFloat)(CTMcontext aContext, CTMenum aProperty);
typedef const CTMuint *(*FP_ctmGetIntegerArray)(CTMcontext aContext,
	CTMenum aProperty);
typedef const CTMfloat *(*FP_ctmGetFloatArray)(CTMcontext aContext,
	CTMenum aProperty);
typedef CTMenum(*FP_ctmGetNamedUVMap)(CTMcontext aContext,
	const char * aName);
typedef const char *(*FP_ctmGetUVMapString)(CTMcontext aContext,
	CTMenum aUVMap, CTMenum aProperty);
typedef CTMfloat(*FP_ctmGetUVMapFloat)(CTMcontext aContext,
	CTMenum aUVMap, CTMenum aProperty);
typedef CTMenum(*FP_ctmGetNamedAttribMap)(CTMcontext aContext,
	const char * aName);
typedef const char *(*FP_ctmGetAttribMapString)(CTMcontext aContext,
	CTMenum aAttribMap, CTMenum aProperty);
typedef CTMfloat(*FP_ctmGetAttribMapFloat)(CTMcontext aContext,
	CTMenum aAttribMap, CTMenum aProperty);
typedef const char *(*FP_ctmGetString)(CTMcontext aContext,
	CTMenum aProperty);
typedef void(*FP_ctmCompressionMethod)(CTMcontext aContext,
	CTMenum aMethod);
typedef void(*FP_ctmCompressionLevel)(CTMcontext aContext,
	CTMuint aLevel);
typedef void(*FP_ctmVertexPrecision)(CTMcontext aContext,
	CTMfloat aPrecision);
typedef void(*FP_ctmVertexPrecisionRel)(CTMcontext aContext,
	CTMfloat aRelPrecision);
typedef void(*FP_ctmNormalPrecision)(CTMcontext aContext,
	CTMfloat aPrecision);
typedef void(*FP_ctmUVCoordPrecision)(CTMcontext aContext,
	CTMenum aUVMap, CTMfloat aPrecision);
typedef void(*FP_ctmAttribPrecision)(CTMcontext aContext,
	CTMenum aAttribMap, CTMfloat aPrecision);
typedef void(*FP_ctmFileComment)(CTMcontext aContext,
	const char * aFileComment);
typedef void(*FP_ctmDefineMesh)(CTMcontext aContext,
	const CTMfloat * aVertices, CTMuint aVertexCount, const CTMuint * aIndices,
	CTMuint aTriangleCount, const CTMfloat * aNormals);
typedef CTMenum(*FP_ctmAddUVMap)(CTMcontext aContext,
	const CTMfloat * aUVCoords, const char * aName, const char * aFileName);
typedef CTMenum(*FP_ctmAddAttribMap)(CTMcontext aContext,
	const CTMfloat * aAttribValues, const char * aName);
typedef void(*FP_ctmLoad)(CTMcontext aContext, const char * aFileName);
typedef void(*FP_ctmLoadCustom)(CTMcontext aContext, CTMreadfn aReadFn,
	void * aUserData);
typedef void(*FP_ctmSave)(CTMcontext aContext, const char * aFileName);
typedef void *(*FP_ctmSaveToBuffer)(CTMcontext aContext, size_t *aBufferSize);
typedef void(*FP_ctmFreeBuffer)(void *buffer);
typedef void(*FP_ctmSaveCustom)(CTMcontext aContext, CTMwritefn aWriteFn,
	void * aUserData);

#include "OpenCTMHandler.generated.h"

UCLASS()
class BIMUNREAL_API AOpenCTMHandler : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AOpenCTMHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// OpenCTM 的 dll文件的路径
	// 静态成员符号声明
	// ----------------
	static FString m_OpenCTMDllPath;

	// 提供赋值给 m_OpenCTMDllPath 的方法
	// ----------------------------------
	static FString GetDllPath();

	// OpenCTM dll 句柄
	// ----------------
	static void * m_OpenCTMDllHandle;

	// 提供赋值给 m_OpenCTMDllHandle 的方法
	// ------------------------------------
	static void * GetCTMDllHandle();

	// OpenCTM 相关
	// ------------
	static void * GetFuncPointer(FString funName);

public:	

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	static FP_ctmNewContext			m_CtmNewContext;
	static FP_ctmFreeContext		m_CtmFreeContext;
	static FP_ctmGetError			m_CtmGetError;
	static FP_ctmErrorString		m_CtmErrorString;
	static FP_ctmGetInteger			m_CtmGetInteger;
	static FP_ctmGetFloat			m_CtmGetFloat;
	static FP_ctmGetIntegerArray	m_CtmGetIntegerArray;
	static FP_ctmGetFloatArray		m_CtmGetFloatArray;
	static FP_ctmGetNamedUVMap		m_CtmGetNamedUVMap;
	static FP_ctmGetUVMapString		m_CtmGetUVMapString;
	static FP_ctmGetUVMapFloat		m_CtmGetUVMapFloat;
	static FP_ctmGetNamedAttribMap	m_CtmGetNamedAttribMap;
	static FP_ctmGetAttribMapString m_CtmGetAttribMapString;
	static FP_ctmGetAttribMapFloat	m_CtmGetAttribMapFloat;
	static FP_ctmGetString			m_CtmGetString;
	static FP_ctmCompressionMethod	m_CtmCompressionMethod;
	static FP_ctmCompressionLevel	m_CtmCompressionLevel;
	static FP_ctmVertexPrecision	m_CtmVertexPrecision;
	static FP_ctmVertexPrecisionRel m_CtmVertexPrecisionRel;
	static FP_ctmNormalPrecision	m_CtmNormalPrecision;
	static FP_ctmUVCoordPrecision	m_CtmUVCoordPrecision;
	static FP_ctmAttribPrecision	m_CtmAttribPrecision;
	static FP_ctmFileComment		m_CtmFileComment;
	static FP_ctmDefineMesh			m_CtmDefineMesh;
	static FP_ctmAddUVMap			m_CtmAddUVMap;
	static FP_ctmAddAttribMap		m_CtmAddAttribMap;
	static FP_ctmLoad				m_CtmLoad;
	static FP_ctmLoadCustom			m_CtmLoadCustom;
	static FP_ctmSave				m_CtmSave;
	static FP_ctmSaveToBuffer		m_CtmSaveToBuffer;
	static FP_ctmFreeBuffer			m_CtmFreeBuffer;
	static FP_ctmSaveCustom			m_CtmSaveCustom;
};
