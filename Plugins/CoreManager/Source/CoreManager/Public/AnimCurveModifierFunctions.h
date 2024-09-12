// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnimCurveModifierFunctions.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FColorAndString
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "ColorAndString")
	FLinearColor Color;

	UPROPERTY(BlueprintReadWrite, Category = "ColorAndString")
	FString Text;

};

UCLASS()
class COREMANAGER_API UAnimCurveModifierFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "AnimCurveUtilities")
	static FColorAndString CreateCurveFunc(TArray<FAssetData> SelectedAssets, FString CurveName, float DefaultValue, bool DontOverride=true);

	UFUNCTION(BlueprintCallable, Category = "AnimCurveUtilities")
	static FColorAndString CopyCurveFunc(TArray<FAssetData> SelectedAssets, FAssetData SourceAnimSequenceData, FString SelectedOption);

	UFUNCTION(BlueprintCallable, Category = "AnimCurveUtilities")
	static FColorAndString RenameCurveFunc(TArray<FAssetData> SelectedAssets, FString SourceName, FString TargetName);

	UFUNCTION(BlueprintCallable, Category = "AnimCurveUtilities")
	static FColorAndString DeleteCurveFunc(TArray<FAssetData> SelectedAssets, FString DeleteCurveName);

};


TMap<FString, TMap<float, float>> ReadAnimData(UAnimSequence* AnimSequenceData);
