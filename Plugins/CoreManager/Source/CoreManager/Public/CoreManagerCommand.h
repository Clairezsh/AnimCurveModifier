// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

/**
 * 
 */
//class COREMANAGER_API CoreManagerCommand
//{
//public:
//	CoreManagerCommand();
//	~CoreManagerCommand();
//};

class FCoreManagerCommand : public TCommands<FCoreManagerCommand>
{
public:
	FCoreManagerCommand():TCommands<FCoreManagerCommand>(
		TEXT("AnimUtils"),
		NSLOCTEXT("Contexts", "AnimUtils", "Animation Utilities Plugin"), 
		NAME_None, 
		FName(TEXT("CoreManagerStyle")))
	{}

	// TCommands<>µÄ½Ó¿Ú£º×¢²áÃüÁî
	virtual void RegisterCommands() override;

public:
	//ÃüÁîA
	//TSharedPtr< FUICommandInfo > CreateACToolWindow;
	TSharedPtr< FUICommandInfo > CreateACToolWindowFromTopMenu;
};
