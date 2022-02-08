// Copyright Epic Games, Inc. All Rights Reserved.

#include "ParagonDuelGameMode.h"
#include "ParagonDuelCharacter.h"
#include "UObject/ConstructorHelpers.h"

AParagonDuelGameMode::AParagonDuelGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
