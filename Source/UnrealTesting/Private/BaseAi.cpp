#include "BaseAi.h"

#include "AIController.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "AIManager.h"

ABaseAi::ABaseAi()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	//Calculate all current stats.
	AiStats.CalculateStatValues();
}

float ABaseAi::GetMaximumHealth()
{
	return AiStats.CalculatedMaximumHealth;
}

void ABaseAi::BeginPlay()
{
	Super::BeginPlay();
	//Calculate all current stats.
	AiStats.CalculateStatValues();
}

void ABaseAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseAi::TakeDamage(float damageAmount, AActor* damagingActor)
{
	AiStats.CurrentHealth -= damageAmount;
	if (AiStats.CurrentHealth <= 0)
	{
		AAIManager::AIManagerInstance->DeleteAi(this);
		return;	
	}

	AAIController* controller = Cast<AAIController>(GetController());
	controller->MoveToLocation(damagingActor->GetActorLocation());
	OnAiHit(damagingActor, damageAmount, AiStats.CurrentHealth);
}

void ABaseAi::FindRandomWanderPoint()
{
}

bool ABaseAi::CheckIfPlayerInVision()
{
	FHitResult lineTraceResult;
	FCollisionObjectQueryParams collisionObjectParams;
	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(this);
	
	if (GetWorld()->LineTraceSingleByObjectType(lineTraceResult, GetActorLocation(), AAIManager::AIManagerInstance->PlayerCharacter->GetActorLocation(), collisionObjectParams ,collisionQueryParams))
	{
		if (lineTraceResult.GetActor() == AAIManager::AIManagerInstance->PlayerCharacter)
		{
			CurrentTarget = lineTraceResult.GetActor();
			return true;
		}
	}
	return false;
}

void ABaseAi::PersuePlayer()
{
}
