// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCoreManagerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TSharedPtr<SWindow>  CreateACToolWindow();
	void AnimCurveToolButtonClicked();

#pragma region ContentBrowserMenuExtension

	void InitCBMenuExtension();

#pragma endregion

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	void AddMenuBarExtend(FMenuBarBuilder& Builder);
	void AddPullDownMenu(FMenuBuilder& Builder);
	

};
