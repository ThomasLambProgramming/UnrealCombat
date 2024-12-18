// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArkhamAttemptGameMode.h"
#include "ArkhamAttemptCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArkhamAttemptGameMode::AArkhamAttemptGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
