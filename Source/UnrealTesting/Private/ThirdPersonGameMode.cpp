#include "ThirdPersonGameMode.h"
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonGameMode::AThirdPersonGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Found the character"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Didnt find the character"));
}
