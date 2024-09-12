// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreManagerCommand.h"

#define LOCTEXT_NAMESPACE "FCoreManagerCommand"


void FCoreManagerCommand::RegisterCommands()
{
	UI_COMMAND(CreateACToolWindowFromTopMenu, "Animation Curve Modifier", "Bring up AnimCurve Tool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
