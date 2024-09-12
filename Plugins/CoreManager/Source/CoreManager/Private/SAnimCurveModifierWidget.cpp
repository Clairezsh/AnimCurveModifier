// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimCurveModifierWidget.h"
#include "SlateOptMacros.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
//#include "DebugHeader.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAnimCurveModifierWidget::Construct(const FArguments& InArgs)
{
#if ENGINE_MAJOR_VERSION == 4
	//FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool = LevelEditorModule.GetFirstLevelEditor()->GetThumbnailPool();

	//MyFunctionForUE4();
#elif ENGINE_MAJOR_VERSION == 5
	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool = UThumbnailManager::Get().GetSharedThumbnailPool();
#endif

#pragma region AnimationAssetRegion
	TSharedPtr<STextBlock> AnimSequenceHeader =
		SNew(STextBlock)
		.ShadowColorAndOpacity(FLinearColor::Black)
		.ShadowOffset(FVector2D(2, 1))
		//.Font(fontstyle)
		//.shadow
		.Text(FText::FromString(TEXT("Select Source Animation")));

	AnimSequencePropertyEntryBox =
		SNew(SObjectPropertyEntryBox)
		.ObjectPath_Raw(this, &SAnimCurveModifierWidget::GetAnimSequencePath)
		.OnObjectChanged_Raw(this, &SAnimCurveModifierWidget::OnAnimSequenceChanged)
		.AllowedClass(UAnimSequence::StaticClass())
		.ThumbnailPool(AssetThumbnailPool)
		.ThumbnailSizeOverride(FIntPoint(60, 60))
		.DisplayThumbnail(true)
		.DisplayUseSelected(true)
		.DisplayBrowse(true)
		.AllowClear(true);

	AnimSequenceBox =
		SNew(SBox)
		.WidthOverride(230)
		//.HeightOverride(150)
		.VAlign(VAlign_Top)
		.Content()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				AnimSequencePropertyEntryBox.ToSharedRef()
			]
		];
#pragma endregion

	Options.Add(MakeShareable(new FString(TEXT("Override All"))));
	Options.Add(MakeShareable(new FString(TEXT("Do Not Override if Curve Exists"))));
	Options.Add(MakeShareable(new FString(TEXT("Only Ovrride if Curve Exists"))));
	SelectedOption = Options[0];
	DontOverride = true;

	TSharedPtr<STextBlock> CreateInfoText = SNew(STextBlock).MinDesiredWidth(150);
	TSharedPtr<STextBlock> CopyInfoText = SNew(STextBlock).MinDesiredWidth(150);
	TSharedPtr<STextBlock> RenameInfoText = SNew(STextBlock).MinDesiredWidth(150);
	TSharedPtr<STextBlock> DeleteInfoText = SNew(STextBlock).MinDesiredWidth(150);

	ChildSlot
	[
        SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(5)
		[
			SNew(SExpandableArea)
			.AreaTitle(FText::FromString("Create Curve"))
			.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			.InitiallyCollapsed(false)
			.BodyContent()
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Black)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(20)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.VAlign(VAlign_Center)
						.Padding(FMargin(0, 5.0f, 0, 5.0f))
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.AutoWidth()
							[
								SNew(SCheckBox)
								.IsChecked(ECheckBoxState::Checked)
								.OnCheckStateChanged_Lambda([=](ECheckBoxState NewState)
								{
									DontOverride = NewState == ECheckBoxState::Checked ? true : false;
								})
							]
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.Padding(FMargin(0, 4.0f, 0, 0))
							.AutoWidth()
							[
								SNew(STextBlock).Text(FText::FromString("Do Not Override If Curve Exists"))
							]
							+ SHorizontalBox::Slot().HAlign(HAlign_Right)
							[
								SNew(SButton)
								.Text(FText::FromString("Clear List"))
								.OnClicked_Lambda([=]() {
									CreateCurveNameBox->SetText(FText::GetEmpty());
									CreateDefaultValueBox->SetText(FText::GetEmpty());
									return FReply::Handled();
								})
							]
						]
						+ SVerticalBox::Slot()
						.VAlign(VAlign_Center)
						.Padding(FMargin(0, 5.0f, 0, 5.0f))
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.AutoWidth()
							[
								SNew(STextBlock).Text(FText::FromString("Curve Name"))
							]
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.Padding(FMargin(5.0f, 0, 0, 0))
							[
								SAssignNew(CreateCurveNameBox, SEditableTextBox)
								.OnTextChanged_Lambda([=](const FText& InText)
								{
									CreateCurveName = InText.ToString();
								})
								.MinDesiredWidth(90)
							]
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Right)
							//.AutoWidth()
							[
								SNew(STextBlock).Text(FText::FromString("Default Value"))
							]
							+ SHorizontalBox::Slot().HAlign(HAlign_Right)
							[
								SAssignNew(CreateDefaultValueBox, SEditableTextBox)
								.OnTextChanged_Lambda([=](const FText& InText)
								{
									CreateDefaultValue = FCString::Atof(*InText.ToString());
								})
								.MinDesiredWidth(90)
							]
						]
						+ SVerticalBox::Slot()
						.VAlign(VAlign_Center)
						.Padding(FMargin(0, 10.0f, 0, 5.0f))
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().HAlign(HAlign_Left)
							[
								CreateInfoText.ToSharedRef()
							]
							+ SHorizontalBox::Slot().HAlign(HAlign_Right)
							[
								SNew(SButton)
								.Text(FText::FromString("Create"))
								.OnClicked_Lambda([=]() {
									if (CreateCurveNameBox->GetText().IsEmpty())
									{
										CreateInfoText->SetColorAndOpacity(FLinearColor::Yellow);
										CreateInfoText->SetText(FText::FromString(TEXT("Please input a curve name.")));
										return FReply::Handled();
									}
									if (CreateDefaultValueBox->GetText().IsEmpty())
									{
										CreateInfoText->SetColorAndOpacity(FLinearColor::Yellow);
										CreateInfoText->SetText(FText::FromString(TEXT("Please input a default value.")));
										return FReply::Handled();
									}

									TArray<FAssetData> SelectedAssets;
									GEditor->GetContentBrowserSelections(SelectedAssets);
									if (SelectedAssets.Num() == 0)
									{
										CreateInfoText->SetColorAndOpacity(FLinearColor::Yellow);
										CreateInfoText->SetText(FText::FromString(TEXT("Please select more than one AnimSequence data.")));
										return FReply::Handled();
									}

									FString CurveName = CreateCurveNameBox->GetText().ToString();
									//FSmartName SmartCurveName;
									FName FCurveName(*CurveName);
									//SmartCurveName.DisplayName = FCurveName;

									FColorAndString ReturnInfo = 
									AnimCurveModifierFunc->CreateCurveFunc(SelectedAssets, CurveName, CreateDefaultValue, DontOverride);

									CreateInfoText->SetColorAndOpacity(ReturnInfo.Color);
									CreateInfoText->SetText(FText::FromString(ReturnInfo.Text));

									return FReply::Handled();
								})
							]
						]
					]

				]
				
			]
	        
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(5)
		[
			SNew(SExpandableArea)
			.AreaTitle(FText::FromString("Copy Curve"))
			.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			.InitiallyCollapsed(false)
			.BodyContent()
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Black)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoHeight()
					.Padding(20,20,20,10)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.MaxWidth(250)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.VAlign(VAlign_Top)
							.MaxHeight(30)
							[
								SNew(STextBlock).Text(FText::FromString("Source Animation"))
							]
							+ SVerticalBox::Slot()
							.VAlign(VAlign_Center)
							.AutoHeight()
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.Padding(5, 5, 5, 5)
								[
									AnimSequenceBox.ToSharedRef()
								]
							]
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSpacer).Size(FVector2D(20, 0)) // Add vertical space of 20 units
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						.AutoWidth()
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.VAlign(VAlign_Top)
							[
								SNew(STextBlock).Text(FText::FromString("Copy Mode"))
							]
							+ SVerticalBox::Slot()
							.VAlign(VAlign_Top)
							[
								SNew(SBox)
								.WidthOverride(200)
								[
									SNew(SComboBox<TSharedPtr<FString>>)
									.OptionsSource(&Options)
									.OnGenerateWidget(this, &SAnimCurveModifierWidget::GenerateComboBoxItem)
									.OnSelectionChanged(this, &SAnimCurveModifierWidget::OnSelectionChanged)
									.InitiallySelectedItem(SelectedOption)
									[
										SNew(STextBlock)
										.Text(this, &SAnimCurveModifierWidget::GetCurrentItemLabel)
									]
								]

							]
						]
					]
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoHeight()
					.Padding(20,10,20,10)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().HAlign(HAlign_Left)
						[
							CopyInfoText.ToSharedRef()
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						[
							SNew(SButton).Text(FText::FromString("Copy"))
							.OnClicked_Lambda([=]() {
								if (AnimSequenceAssetData == nullptr)
								{
									CreateInfoText->SetColorAndOpacity(FLinearColor::Yellow);
									CreateInfoText->SetText(FText::FromString(TEXT("Please select a source AnimSequence.")));
									return FReply::Handled();
								}

								TArray<FAssetData> SelectedAssets;
								GEditor->GetContentBrowserSelections(SelectedAssets);
								if (SelectedAssets.Num() == 0)
								{
									CreateInfoText->SetColorAndOpacity(FLinearColor::Yellow);
									CreateInfoText->SetText(FText::FromString(TEXT("Please select more than one AnimSequence data.")));
									return FReply::Handled();
								}

								FColorAndString ReturnInfo =
								AnimCurveModifierFunc->CopyCurveFunc(SelectedAssets, AnimSequenceAssetData, *SelectedOption);

								CopyInfoText->SetColorAndOpacity(ReturnInfo.Color);
								CopyInfoText->SetText(FText::FromString(ReturnInfo.Text));

								return FReply::Handled();
							})
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(5)
		[
			SNew(SExpandableArea)
				.AreaTitle(FText::FromString("Rename Curve"))
				.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Regular", 10))
				.InitiallyCollapsed(false)
				.BodyContent()
				[
					SNew(SBorder)
					.BorderBackgroundColor(FLinearColor::Black)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.VAlign(VAlign_Center)
						.Padding(20)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.VAlign(VAlign_Center)
							.Padding(FMargin(0, 5.0f, 0, 5.0f))
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot().HAlign(HAlign_Right)
								[
									SNew(SButton)
									.Text(FText::FromString("Clear List"))
									.OnClicked_Lambda([=]() {
										RenameSourceNameBox->SetText(FText::GetEmpty());
										RenameTargetNameBox->SetText(FText::GetEmpty());
										RenameSourceName = FString();
										RenameTargetName = FString();
										return FReply::Handled();
									})
								]
							]
							+ SVerticalBox::Slot()
							.VAlign(VAlign_Center)
							.Padding(FMargin(0, 5.0f, 0, 5.0f))
							[
								SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Left)
									.AutoWidth()
									[
										SNew(STextBlock).Text(FText::FromString("Source Name"))
									]
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Left)
									.Padding(FMargin(5.0f, 0, 0, 0))
									[
										SAssignNew(RenameSourceNameBox, SEditableTextBox)
										.OnTextChanged_Lambda([=](const FText& InText)
										{
											RenameSourceName = InText.ToString();
										})
										.MinDesiredWidth(90)
									]
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Right)
									//.AutoWidth()
									[
										SNew(STextBlock).Text(FText::FromString("Target Name"))
									]
									+ SHorizontalBox::Slot().HAlign(HAlign_Right)
									[
										SAssignNew(RenameTargetNameBox, SEditableTextBox)
										.OnTextChanged_Lambda([=](const FText& InText)
										{
											RenameTargetName = InText.ToString();
										})
										.MinDesiredWidth(90)
									]
							]
							+ SVerticalBox::Slot()
							.VAlign(VAlign_Center)
							.Padding(FMargin(0, 10.0f, 0, 5.0f))
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot().HAlign(HAlign_Left)
								[
									RenameInfoText.ToSharedRef()
								]
								+ SHorizontalBox::Slot().HAlign(HAlign_Right)
								[
									SNew(SButton).Text(FText::FromString("Rename"))
									.OnClicked_Lambda([=]() {
										if (RenameSourceName.IsEmpty())
										{
											RenameInfoText->SetColorAndOpacity(FLinearColor::Yellow);
											RenameInfoText->SetText(FText::FromString(TEXT("Please input source name.")));
											return FReply::Handled();
										}
										if (RenameTargetName.IsEmpty())
										{
											RenameInfoText->SetColorAndOpacity(FLinearColor::Yellow);
											RenameInfoText->SetText(FText::FromString(TEXT("Please input target name.")));
											return FReply::Handled();
										}
										
										TArray<FAssetData> SelectedAssets;
										GEditor->GetContentBrowserSelections(SelectedAssets);
										if (SelectedAssets.Num() == 0)
										{
											RenameInfoText->SetColorAndOpacity(FLinearColor::Yellow);
											RenameInfoText->SetText(FText::FromString(TEXT("Please select more than one AnimSequence data.")));
											return FReply::Handled();
										}

										FColorAndString ReturnInfo =
										AnimCurveModifierFunc->RenameCurveFunc(SelectedAssets, RenameSourceName, RenameTargetName);

										RenameInfoText->SetColorAndOpacity(ReturnInfo.Color);
										RenameInfoText->SetText(FText::FromString(ReturnInfo.Text));

										return FReply::Handled();
									})
								]
							]
					]

				]

			]

		]
		+ SVerticalBox::Slot().AutoHeight().Padding(5)
		[
			SNew(SExpandableArea)
			.AreaTitle(FText::FromString("Delete Curve"))
			.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			.InitiallyCollapsed(false)
			.BodyContent()
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Black)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(20)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.VAlign(VAlign_Center)
						.Padding(FMargin(0, 5.0f, 0, 5.0f))
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.AutoWidth()
							[
								SNew(STextBlock).Text(FText::FromString("Curve Name"))
							]
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.Padding(FMargin(5.0f, 0, 0, 0))
							[
								SAssignNew(DeleteCurveNameBox, SEditableTextBox)
								.MinDesiredWidth(90)
								.OnTextChanged_Lambda([=](const FText& InText)
								{
									DeleteCurveName = InText.ToString();
								})
							]
							+ SHorizontalBox::Slot().HAlign(HAlign_Right)
							[
								SNew(SButton)
								.Text(FText::FromString("Clear List"))
								.OnClicked_Lambda([=]() {
									DeleteCurveNameBox->SetText(FText::GetEmpty());
									DeleteCurveName = FString();
									return FReply::Handled();
								})
							]
						]
						+ SVerticalBox::Slot()
						.VAlign(VAlign_Center)
						.Padding(FMargin(0, 10.0f, 0, 5.0f))
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().HAlign(HAlign_Left)
							[
								DeleteInfoText.ToSharedRef()
							]
							+ SHorizontalBox::Slot().HAlign(HAlign_Right)
							[
								SNew(SButton).Text(FText::FromString("Delete"))
								.OnClicked_Lambda([=]() {
									if (DeleteCurveName.IsEmpty())
									{
										DeleteInfoText->SetColorAndOpacity(FLinearColor::Yellow);
										DeleteInfoText->SetText(FText::FromString(TEXT("Please input curve name to delete.")));
										return FReply::Handled();
									}

									TArray<FAssetData> SelectedAssets;
									GEditor->GetContentBrowserSelections(SelectedAssets);
									if (SelectedAssets.Num() == 0)
									{
										DeleteInfoText->SetColorAndOpacity(FLinearColor::Yellow);
										DeleteInfoText->SetText(FText::FromString(TEXT("Please select more than one AnimSequence data.")));
										return FReply::Handled();
									}

									FColorAndString ReturnInfo =
									AnimCurveModifierFunc->DeleteCurveFunc(SelectedAssets, DeleteCurveName);

									DeleteInfoText->SetColorAndOpacity(ReturnInfo.Color);
									DeleteInfoText->SetText(FText::FromString(ReturnInfo.Text));

									return FReply::Handled();
								})
							]
						]
					]

				]

			]

		]
	];

}

FString SAnimCurveModifierWidget::GetAnimSequencePath() const
{
	if (AnimSequenceAssetData.IsValid())
	{
#if UE_VERSION_OLDER_THAN(5,1,0)
		return AnimSequenceAssetData.ObjectPath.ToString();
#else
		return AnimSequenceAssetData.GetObjectPathString();
#endif
	}
	else return "";
}

bool SAnimCurveModifierWidget::ShouldShowAnimSequenceResetToDefault(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	//PropertyHandle.Get()->
	return GetAnimSequencePath() != "";
}
void SAnimCurveModifierWidget::ResetAnimSequenceToDefault(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	//PropertyHandle->a
	AnimSequenceAssetData = nullptr;
}

void SAnimCurveModifierWidget::OnAnimSequenceChanged(const FAssetData& AssetData)
{
	AnimSequenceAssetData = AssetData;
}

TSharedRef<SWidget> SAnimCurveModifierWidget::GenerateComboBoxItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

void SAnimCurveModifierWidget::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type)
{
	SelectedOption = NewValue;
}

FText SAnimCurveModifierWidget::GetCurrentItemLabel() const
{
	return FText::FromString(*SelectedOption);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
