// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboBox.h"
#include "Misc/EngineVersionComparison.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#include "AnimCurveModifierFunctions.h"

/**
 * 
 */
class COREMANAGER_API SAnimCurveModifierWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAnimCurveModifierWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<SObjectPropertyEntryBox> AnimSequencePropertyEntryBox;
	TSharedPtr<SBox> AnimSequenceBox;
	

private:

	FString CreateCurveName;
	float CreateDefaultValue;
	bool DontOverride;
	TSharedPtr<SEditableTextBox> CreateCurveNameBox;
	TSharedPtr<SEditableTextBox> CreateDefaultValueBox;


	TArray<TSharedPtr<FString>> Options;
	TSharedPtr<FString> SelectedOption;
	FAssetData AnimSequenceAssetData;

	FString RenameSourceName;
	FString RenameTargetName;
	TSharedPtr<SEditableTextBox> RenameSourceNameBox;
	TSharedPtr<SEditableTextBox> RenameTargetNameBox;

	FString DeleteCurveName;
	TSharedPtr<SEditableTextBox> DeleteCurveNameBox;

	FString GetAnimSequencePath() const;

	
	bool ShouldShowAnimSequenceResetToDefault(TSharedPtr<IPropertyHandle> PropertyHandle);
	void ResetAnimSequenceToDefault(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnAnimSequenceChanged(const FAssetData& AssetData);

    // Copy mode


    TSharedRef<SWidget> GenerateComboBoxItem(TSharedPtr<FString> InItem);

	void OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type);

	FText GetCurrentItemLabel() const;

	UAnimCurveModifierFunctions* AnimCurveModifierFunc;
};
