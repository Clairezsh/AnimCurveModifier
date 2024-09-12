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

	// TCommands<>�Ľӿڣ�ע������
	virtual void RegisterCommands() override;

public:
	//����A
	//TSharedPtr< FUICommandInfo > CreateACToolWindow;
	TSharedPtr< FUICommandInfo > CreateACToolWindowFromTopMenu;
};
