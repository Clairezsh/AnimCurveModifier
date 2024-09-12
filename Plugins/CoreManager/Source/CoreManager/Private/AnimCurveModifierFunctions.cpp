// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimCurveModifierFunctions.h"
#include "Misc/EngineVersionComparison.h"
#include "UObject/SavePackage.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "FileHelpers.h"
#include "UObject/ConstructorHelpers.h"

FColorAndString UAnimCurveModifierFunctions::CreateCurveFunc(TArray<FAssetData> SelectedAssets, FString CurveName, float DefaultValue, bool DontOverride)
{
	FColorAndString ReturnInfo;

	FName FCurveName(*CurveName);

	for (const FAssetData& AssetData : SelectedAssets)
	{
		TArray<FString> AllCurveName;
		if (UAnimSequence* AnimSequence = Cast<UAnimSequence>(AssetData.GetAsset()))
		{
			TMap<FString, TMap<float, float>> FrameCurveValues;
			IAnimationDataController& Controller = AnimSequence->GetController();

			USkeleton* Skeleton = AnimSequence->GetSkeleton();
			if (!Skeleton)
			{
				ReturnInfo.Color = FLinearColor::Yellow;
				ReturnInfo.Text = TEXT("Skeleton of this AnimSequence is null.");
				return ReturnInfo;
			}

			FSmartName SmartName;
			bool found = Skeleton->GetSmartNameByName(USkeleton::AnimCurveMappingName, FCurveName, SmartName);
			if (!found)
			{
				Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, FCurveName, SmartName);
			}
			//SmartName.DisplayName = FCurveName;
			IAnimationDataController& AnimDataController = AnimSequence->GetController();
			FAnimationCurveIdentifier CurveId(SmartName, ERawCurveTrackTypes::RCT_Float);
			AnimDataController.OpenBracket(FText::FromString(TEXT("SetCurveValue")));
			if (!found)
			{
				//Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, FCurveName, SmartName);
				AnimDataController.AddCurve(CurveId);
			}
			else
			{
				if (DontOverride) {
					ReturnInfo.Color = FLinearColor::Yellow;
					ReturnInfo.Text = TEXT("Curve already exists!");
					return ReturnInfo;
				}
			}

			AnimDataController.SetCurveKey(CurveId, FRichCurveKey(0, DefaultValue));
			AnimDataController.CloseBracket();
		}
	}

	ReturnInfo.Color = FLinearColor::Green;
	ReturnInfo.Text = TEXT("Create successfully!");
	return ReturnInfo;
}

TMap<FString, TMap<float, float>> ReadAnimData(UAnimSequence* AnimSequenceData)
{

	float fps = AnimSequenceData->GetDataModel()->GetFrameRate().AsDecimal();

	int32 TotalFrames = AnimSequenceData->GetNumberOfSampledKeys();

	TMap<FString, TMap<float, float>> FrameCurveValues;

	const TArray<FFloatCurve>& AnimationCurves = AnimSequenceData->GetDataModel()->GetFloatCurves();

	for (const FFloatCurve& Curve : AnimationCurves)
	{
		TMap<float, float> CurveValues;

		for (const FRichCurveKey& Key : Curve.FloatCurve.Keys)
		{
			float CurveValue = Curve.Evaluate(Key.Time);
			CurveValues.Add(Key.Time, CurveValue);
		}
		FrameCurveValues.Add(Curve.Name.DisplayName.ToString(), CurveValues);
	}

	return FrameCurveValues;
}

FColorAndString UAnimCurveModifierFunctions::CopyCurveFunc(TArray<FAssetData> SelectedAssets, FAssetData SourceAnimSequenceData, FString SelectedOption)
{
	FColorAndString ReturnInfo;

	UAnimSequence* SourceAnimSequence = Cast<UAnimSequence>(SourceAnimSequenceData.GetAsset());
	TMap<FString, TMap<float, float>> SourceAnimData = ReadAnimData(SourceAnimSequence);

	for (const FAssetData& AssetData : SelectedAssets)
	{
		UAnimSequence* AnimSequence = Cast<UAnimSequence>(AssetData.GetAsset());

		// set fps

		float fps = SourceAnimSequence->GetDataModel()->GetFrameRate().AsDecimal();

		int32 EndFrame = SourceAnimSequence->GetNumberOfSampledKeys();

		const float DurationInSeconds = EndFrame / float(fps);// 
		AnimSequence->ImportFileFramerate = float(fps);
		AnimSequence->ImportResampleFramerate = fps;

		IAnimationDataController& Controller = AnimSequence->GetController();
		Controller.OpenBracket(FText::FromString(TEXT("InitializeAnimation")));
		Controller.InitializeModel();
		Controller.SetFrameRate(FFrameRate(fps, 1), true);
		Controller.SetNumberOfFrames(FFrameRate(fps, 1).AsFrameNumber(FMath::Max<float>(DurationInSeconds, MINIMUM_ANIMATION_LENGTH)), true);

		USkeleton* Skeleton = AnimSequence->GetSkeleton();
		if (!Skeleton)
		{
			ReturnInfo.Color = FLinearColor::Yellow;
			ReturnInfo.Text = TEXT("Found no Skeleton, invalid AnimSequence!");
			return ReturnInfo;
		}

		for (auto& AnimValue : SourceAnimData)
		{
			FName CurveName = FName(*AnimValue.Key);
			TMap<float, float> CurveKeyValue = AnimValue.Value;
			FSmartName NewTrackName;
			FSmartName NewName;
			
			bool FoundCurve = Skeleton->GetSmartNameByName(USkeleton::AnimCurveMappingName, CurveName, NewName);
			if (SelectedOption == FString(TEXT("Do Not Override if Curve Exists")))
			{
				if (FoundCurve){ continue; }
			}
			else if (SelectedOption == FString(TEXT("Only Ovrride if Curve Exists")))
			{
				if (!FoundCurve) { continue; }
			}
			Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, CurveName, NewTrackName);
			if (NewTrackName.IsValid())
			{
				AnimSequence->Modify();

				ensureAlways(Skeleton->GetSmartNameByUID(USkeleton::AnimCurveMappingName, NewTrackName.UID, NewName));
				const FAnimationCurveIdentifier FloatCurveId(NewName, ERawCurveTrackTypes::RCT_Float);
				Controller.AddCurve(FloatCurveId);	
			}

			const FAnimationCurveIdentifier FloatCurveId(NewName, ERawCurveTrackTypes::RCT_Float);
			TArray<FRichCurveKey> currentCurveKeys;
			for (auto& kv : CurveKeyValue)
			{
				float time = kv.Key;
				float keyvalue = kv.Value;
				currentCurveKeys.Add(FRichCurveKey(time, keyvalue));
			}
			Controller.SetCurveKeys(FloatCurveId, currentCurveKeys, false);
		}
		Controller.NotifyPopulated();
		Controller.CloseBracket();
	}

	ReturnInfo.Color = FLinearColor::Green;
	ReturnInfo.Text = TEXT("Copy done!");
	return ReturnInfo;
}

FColorAndString UAnimCurveModifierFunctions::RenameCurveFunc(TArray<FAssetData> SelectedAssets, FString SourceName, FString TargetName)
{
	FColorAndString ReturnInfo;
	for (const FAssetData& AssetData : SelectedAssets)
	{
		UAnimSequence* AnimSequence = Cast<UAnimSequence>(AssetData.GetAsset());

		IAnimationDataModel* DataModel = AnimSequence->GetDataModel();

		USkeleton* Skeleton = AnimSequence->GetSkeleton();
		// Find id of source curve 
		FSmartName SourceSmartName;
		const FName SourceFName(*SourceName);
		if (!Skeleton->GetSmartNameByName(USkeleton::AnimCurveMappingName, SourceFName, SourceSmartName))
		{
			ReturnInfo.Color = FLinearColor::Yellow;
			ReturnInfo.Text = TEXT("Source curve name ") + SourceName + TEXT(" not found.");
			return ReturnInfo;
		}
		FAnimationCurveIdentifier SourceCurveId(SourceSmartName, ERawCurveTrackTypes::RCT_Float);
		const FFloatCurve* SourceCurve = DataModel->FindFloatCurve(SourceCurveId);  //find source curve

		// Find id of target curve 
		FSmartName TargetSmartName;
		const FName TargetFName(*TargetName);
		if (!Skeleton->GetSmartNameByName(USkeleton::AnimCurveMappingName, TargetFName, TargetSmartName))
		{
			Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, TargetFName, TargetSmartName);
			FAnimationCurveIdentifier TargetCurveId(TargetSmartName, ERawCurveTrackTypes::RCT_Float);
			
			// Rename
			IAnimationDataController& DataController = AnimSequence->GetController();
			DataController.RenameCurve(SourceCurveId, TargetCurveId);
		}
		else
		{
			ReturnInfo.Color = FLinearColor::Yellow;
			ReturnInfo.Text = TEXT("Target curve name ") + TargetName + TEXT("has already exist.");
			return ReturnInfo;
		}
	}

	ReturnInfo.Color = FLinearColor::Green;
	ReturnInfo.Text = TEXT("Rename successfully!");
	return ReturnInfo;
}

FColorAndString UAnimCurveModifierFunctions::DeleteCurveFunc(TArray<FAssetData> SelectedAssets, FString DeleteCurveName)
{
	FColorAndString ReturnInfo;
	for (const FAssetData& AssetData : SelectedAssets)
	{
		UAnimSequence* AnimSequence = Cast<UAnimSequence>(AssetData.GetAsset());

		IAnimationDataModel* DataModel = AnimSequence->GetDataModel();

		USkeleton* Skeleton = AnimSequence->GetSkeleton();
		// Find id of source curve 
		FSmartName DeleteSmartName;
		const FName DeleteFName(*DeleteCurveName);
		bool FoundCurve = Skeleton->GetSmartNameByName(USkeleton::AnimCurveMappingName, DeleteFName, DeleteSmartName);
		if (!FoundCurve)
		{
			ReturnInfo.Color = FLinearColor::Yellow;
			ReturnInfo.Text = TEXT("Curve ") + DeleteCurveName + TEXT(" not found.");
			return ReturnInfo;
		}

		// Delete Curve
		IAnimationDataController& DataController = AnimSequence->GetController();
		FAnimationCurveIdentifier TargetCurveId(DeleteSmartName, ERawCurveTrackTypes::RCT_Float);
		DataController.RemoveCurve(TargetCurveId);
	}

	ReturnInfo.Color = FLinearColor::Green;
	ReturnInfo.Text = TEXT("Delete successfully!");
	return ReturnInfo;
}
