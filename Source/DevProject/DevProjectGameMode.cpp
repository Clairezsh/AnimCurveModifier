// Copyright Epic Games, Inc. All Rights Reserved.

#include "DevProjectGameMode.h"
#include "DevProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADevProjectGameMode::ADevProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
