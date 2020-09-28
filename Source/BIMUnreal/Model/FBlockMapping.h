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

	// ԭ��������һ���洢�����ݸ�ʽ��
	// ---------------------------
	TArray<TSharedPtr<FJsonValue>> blockMappingItems;

	// �����������ĸ�ʽ
	// ----------------
	TArray<FBlockMapping_Item> blockMappingItems_objlist;
};
