// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CoreManager : ModuleRules
{
	public CoreManager(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Source/Editor/Blutility/Private",
				System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Source/Developer/AssetTools/Private",
				System.IO.Path.Combine(GetModuleDirectory("SceneOutliner"), "Private")
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core","Blutility","EditorScriptingUtilities","ContentBrowser", "EditorSubsystem","MainFrame","UnrealEd",
                "EditorFramework",
                "ActorPickerMode",
                "SceneDepthPickerMode",
                "EditorConfig",
                "InputCore"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"AssetTools",
				"PropertyEditor"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
