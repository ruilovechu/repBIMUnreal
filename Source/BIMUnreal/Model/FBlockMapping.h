// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "FBlockMapping_Item.h"

/**
 * 
 */
class BIMUNREAL_API FBlockMapping
{
public:

	// 原来本身（上一级存储的数据格式）
	// ---------------------------
	TArray<TSharedPtr<FJsonValue>> blockMappingItems;

	// 解析后真正的格式
	// ----------------
	TArray<FBlockMapping_Item> blockMappingItems_objlist;
};
