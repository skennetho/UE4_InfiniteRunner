// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfiniteRunnerGameMode.h"
#include "InfiniteRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInfiniteRunnerGameMode::AInfiniteRunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
