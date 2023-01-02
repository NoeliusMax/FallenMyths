// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestCodiGameMode.h"
#include "TestCodiCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestCodiGameMode::ATestCodiGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
