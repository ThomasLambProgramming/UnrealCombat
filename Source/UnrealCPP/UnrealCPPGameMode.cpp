// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealCPPGameMode.h"
#include "UnrealCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealCPPGameMode::AUnrealCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
