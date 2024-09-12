// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreManager.h"
//#include "DebugHeader.h"
#include "CoreManagerCommand.h"
#include "SAnimCurveModifierWidget.h"

#include "ContentBrowserModule.h"
#include "LevelEditor.h"


#define LOCTEXT_NAMESPACE "FCoreManagerModule"

TWeakPtr<SWindow> G_CoreLinkWindow;

void FCoreManagerModule::StartupModule()
{
	FCoreManagerCommand::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	//PluginCommands->MapAction(
	//	FCoreManagerCommand::Get().CreateACToolWindow,
	//	FExecuteAction::CreateRaw(this, &FCoreManagerModule::AnimCurveToolButtonClicked),
	//	FCanExecuteAction());

	PluginCommands->MapAction(
		FCoreManagerCommand::Get().CreateACToolWindowFromTopMenu,
		FExecuteAction::CreateRaw(this, &FCoreManagerModule::AnimCurveToolButtonClicked),
		FCanExecuteAction());

	InitCBMenuExtension();
}

#pragma region CustomToolMenuExtension

void FCoreManagerModule::InitCBMenuExtension()
{
	// Ìí¼Ó²Ëµ¥À¸À©Õ¹
	FLevelEditorModule& LevelEditorModule = 
	FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> MenuBarExtend = MakeShareable(new FExtender());
	MenuBarExtend->AddMenuBarExtension("Help", EExtensionHook::After, PluginCommands, FMenuBarExtensionDelegate::CreateRaw(this, &FCoreManagerModule::AddMenuBarExtend));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuBarExtend);

	FContentBrowserModule& ContentBrowserModule =
	FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	ContentBrowserModule.GetAllPathViewContextMenuExtenders();
}

void FCoreManagerModule::AddMenuBarExtend(FMenuBarBuilder& Builder)
{
	Builder.AddPullDownMenu(
		FText::FromString("CostomUtils"),
		FText::FromString("Costom Utilities"),
		FNewMenuDelegate::CreateRaw(this, &FCoreManagerModule::AddPullDownMenu));
}

void FCoreManagerModule::AddPullDownMenu(FMenuBuilder& Builder)
{
	//Print(TEXT("Working pull down"), FColor::Cyan);
	Builder.BeginSection("AnimUtils", NSLOCTEXT("AnimUtilsMenu", "AnimUtils", "AnimUtils"));
	Builder.AddMenuEntry(FCoreManagerCommand::Get().CreateACToolWindowFromTopMenu);

	Builder.EndSection();
}


void FCoreManagerModule::AnimCurveToolButtonClicked()
{
	TSharedPtr<SWindow> ExistingWindow = G_CoreLinkWindow.Pin();
	if (ExistingWindow.IsValid())
	{
		ExistingWindow->BringToFront();
	}
	else
	{
		ExistingWindow = CreateACToolWindow();
	}
	G_CoreLinkWindow = ExistingWindow;
}

TSharedPtr<SWindow> FCoreManagerModule::CreateACToolWindow()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedRef<FTabManager> TabManager = LevelEditorModule.GetLevelEditorTabManager().ToSharedRef();

	float WindowWidth = 500;
	float WindowHight = 705;

#pragma region AnimCurveModifierUI

	TSharedPtr<SWindow> ExistingWindow = SNew(SWindow)
		.Title(LOCTEXT("Animation Curve Modifier", "Animation Curve Modifier"))
		.SizingRule(ESizingRule::FixedSize)
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.Clipping(EWidgetClipping::ClipToBoundsAlways)
		.ClientSize(FVector2D(WindowWidth, WindowHight))
		.AdjustInitialSizeAndPositionForDPIScale(true);

	ExistingWindow->SetContent(SNew(SAnimCurveModifierWidget));
#pragma endregion

	TSharedPtr<SDockTab> OwnerTab = TabManager->GetOwnerTab();
	TSharedPtr<SWindow> RootWindow = OwnerTab.IsValid() ? OwnerTab->GetParentWindow() : TSharedPtr<SWindow>();
	if (RootWindow.IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(ExistingWindow.ToSharedRef(), RootWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(ExistingWindow.ToSharedRef());
	}

	return ExistingWindow;
}

#pragma endregion

void FCoreManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCoreManagerModule, CoreManager)