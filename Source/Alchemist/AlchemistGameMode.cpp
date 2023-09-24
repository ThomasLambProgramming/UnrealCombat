// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemistGameMode.h"
#include "AlchemistCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAlchemistGameMode::AAlchemistGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
