// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BIMUNREAL_API FMaterialContent
{
public:
	FString color;
	FString appearancecolor;
	bool tinttoggle;
	FString tintcolor;
	double tp;
	int shi;
	FString cat;
	bool userenderappearance;
	FString img;
	double uf;
	double vf;
	double usc;
	double vsc;
	double uvsc;
	double ang;
	bool ismetal;
	bool URepeat;
	bool VRepeat;
	FString typ;
	bool rpc;
	FString rpccat;
	FString rpcid;
	double rpcheight;
	bool rpcreflections;
	bool rpcshadows;
	TArray<FString> othertextures;
	FString SelfIllumFiltermap;
	double SelfIllumLuminance;
	double SelfIllumColorTemperature;
	double ReflectivityDirect;
	double ReflectivityOblique;
	double Glossness;
};
