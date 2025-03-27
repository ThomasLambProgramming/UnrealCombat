#include "ThirdPersonController.h"
#include "AIManager.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AThirdPersonController::AThirdPersonController()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->TargetOffset = FVector(0,0,100);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AThirdPersonController::BeginPlay()
{
	Super::BeginPlay();
	currentSpellType = slotOneProjectile.GetDefaultObject();
	attackLerpingMaxDistanceSquared = attackLerpingMaxDistance * attackLerpingMaxDistance;
	AActor* aiManager = UGameplayStatics::GetActorOfClass(GetWorld(), AAIManager::StaticClass());
	//AActor* projectileManager = UGameplayStatics::GetActorOfClass(GetWorld(), AProjectileManager::StaticClass());
	//ProjectileManager = Cast<AProjectileManager>(projectileManager);
	AiManager = Cast<AAIManager>(aiManager);
}

void AThirdPersonController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	shootingTimer += DeltaSeconds;
	ProcessLerpDash();
	if (!IsDashAttacking)
		FindClosestEnemy();

	if (InCannonAnimation)
		SetActorRotation(rotationOverrideWhileFiring.ToOrientationRotator());
	
}

bool AThirdPersonController::FindClosestEnemy()
{
	if (CurrentInputDirection.Length() < 0.1f || AiManager->AiActorsInMap.Num() <= 0)
	{
		selectedAiIndex = -1;	
		return false;
	}
	
	FVector cameraForwardDirection = FollowCamera->GetComponentToWorld().GetRotation().GetForwardVector();
	cameraForwardDirection.Z = 0;
	cameraForwardDirection.Normalize();
	FVector cameraRightDirection = FollowCamera->GetComponentToWorld().GetRotation().GetRightVector();
	cameraRightDirection.Z = 0;
	cameraRightDirection.Normalize();
	
	FVector inputDirectionInWorld = (cameraForwardDirection * CurrentInputDirection.Y + cameraRightDirection * CurrentInputDirection.X);
	inputDirectionInWorld = inputDirectionInWorld.GetSafeNormal();

	
	//DrawDebugSphere(GetWorld(), inputDirectionInWorld * 100 + GetActorLocation(), 30, 20, FColor::Cyan);
	int closestEnemy = -1000;
	float previousDot = FLT_MIN;
	for (int i = 0; i < AiManager->AiActorsInMap.Num(); ++i)
	{
		
		FVector difference = AiManager->AiActorsInMap[i]->GetActorLocation() - GetActorLocation(); 
		float distance = difference.SquaredLength();
		if (distance > attackLerpingMaxDistanceSquared)
			continue;
		
		float dotProduct = inputDirectionInWorld.Dot(difference.GetSafeNormal());
		if (dotProduct > previousDot && dotProduct > MinimumAllowedDashDotProduct)
		{
			previousDot = dotProduct;
			closestEnemy = i;
			GEngine->AddOnScreenDebugMessage(i + 700, 5, FColor::Blue, TEXT("New closest given:") + FString::FromInt(dotProduct * 100) + FString::FromInt(closestEnemy));
		}
	}
	if (closestEnemy <= -1)
	{
		selectedAiIndex = -1;	
		return false;
	}

	DrawDebugSphere(GetWorld(), AiManager->AiActorsInMap[closestEnemy]->GetActorLocation(), 30, 20, FColor::Cyan);
	selectedAiIndex = closestEnemy;
	return true;
}

void AThirdPersonController::ProcessLerpDash()
{
	if (!IsDashAttacking || selectedAiIndex <= -1 && AiManager->AiActorsInMap.Num() <= 0)
		return;
	
	GEngine->AddOnScreenDebugMessage(345978057, 5, FColor::Blue, TEXT("Lerping Tick"));
	attackTimer += GetWorld()->DeltaTimeSeconds;
	FVector currentActorLocation = GetActorLocation();
	if (FVector::Distance(currentActorLocation, AiManager->AiActorsInMap[selectedAiIndex]->GetActorLocation()) < attackLerpingMinDistance)
	{
		IsDashAttacking = false;
		AiManager->DeleteAi(selectedAiIndex);
		selectedAiIndex = 0;
	}
	else
	{
		//lerp between where we started the attack and where the ai enemy currently is based on the powercurve so extra "style" can be added and modified without changing code.
		float getLerpAmount = powerCurveOnHit->GetFloatValue(attackTimer / attackDuration);
		FVector lerpGoalLocation = AiManager->AiActorsInMap[selectedAiIndex]->GetActorLocation();
		//for now since it is a flat surface the z location has to be the same since the mesh offsets are ever so slightly off causing the player to be a few units into the ground.
		lerpGoalLocation.Z = startingLocation.Z;
		FRotator goalRotation = (lerpGoalLocation - GetActorLocation()).ToOrientationRotator();
		SetActorRotation(FMath::Lerp(GetActorRotation(), goalRotation, getLerpAmount));
		SetActorLocation(FMath::Lerp(startingLocation, lerpGoalLocation, getLerpAmount));
		//We dont need to do ai detection if we are already going to attack an enemy.
		if (attackTimer >= attackDuration)
		{
			IsDashAttacking = false;
			AiManager->DeleteAi(selectedAiIndex);
			selectedAiIndex = 0;
		}
	}
}

void AThirdPersonController::FireEquippedSpell()
{
	TSubclassOf<AProjectile> projectileToFire;
	switch (currentlySelectedSlot)
	{
	case 0:
		projectileToFire = slotOneProjectile;
		break;
	case 1:
		projectileToFire = slotTwoProjectile;
		break;
	case 2:
		projectileToFire = slotThreeProjectile;
		break;
	default:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(4, 5, FColor::Red, TEXT("Tried to fire an unknown projectile slot, fix!"));
		}
	}
	
	FHitResult hitResult;
	FVector raycastEndLocation = GetFollowCamera()->GetComponentLocation() + GetFollowCamera()->GetForwardVector() * 2000;
	FCollisionQueryParams traceParams = FCollisionQueryParams(TEXT("CrosshairTrace"), true, this);
	GetWorld()->LineTraceSingleByChannel(hitResult, GetFollowCamera()->GetComponentLocation(), raycastEndLocation, TraceChannelProperty, traceParams);
	FVector hitLocation;
	if (hitResult.bBlockingHit)
		hitLocation = hitResult.Location;
	else
		hitLocation = raycastEndLocation;
	DrawDebugSphere(GetWorld(), hitLocation, 20, 10, FColor::Red);

	FVector defaultProjectileLocation = GetActorLocation() + GetFollowCamera()->GetForwardVector() * 80;
	FVector directionToFire = hitLocation - defaultProjectileLocation;
	directionToFire.Normalize();
	rotationOverrideWhileFiring = directionToFire;
	rotationOverrideWhileFiring.Z = 0;
	rotationOverrideWhileFiring.Normalize();
	
	FRotator defaultProjectileRotation = directionToFire.ToOrientationRotator();
	FActorSpawnParameters defaultProjectileSpawnParams;
	for (int i = 0; i < currentSpellType->Multishot; ++i)
	{
		FVector positionOffet = GetFollowCamera()->GetUpVector().RotateAngleAxis((360.0f / currentSpellType->Multishot) * i, GetFollowCamera()->GetForwardVector());

		AProjectile* test = GetWorld()->SpawnActor<AProjectile>(projectileToFire, defaultProjectileLocation + (currentSpellType->Multishot > 1 ? (positionOffet * 50) : FVector(0,0,0)), defaultProjectileRotation, defaultProjectileSpawnParams);
		test->ProjectileMovement->bInterpMovement = false;
		test->ProjectileMovement->ResetInterpolation();
		test->ProjectileMovement->SetUpdatedComponent(test->GetRootComponent());
		test->ProjectileMovement->Velocity = directionToFire * currentSpellType->Speed;
		//Give the new projectile the ai manager reference to track enemies.
		test->SetupProjectile(AiManager);
	}
}


void AThirdPersonController::Move(const FInputActionValue& Value)
{
	//Attacking should not be able to be changed once the attack has started.
	if (IsDashAttacking)
		return;
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	CurrentInputDirection = MovementVector;
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y).GetSafeNormal();

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
void AThirdPersonController::MoveCancelled(const FInputActionValue& Value)
{
	CurrentInputDirection = FVector2D(0,0);
}

void AThirdPersonController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThirdPersonController::Attack(const FInputActionValue& Value)
{
	if (IsDashAttacking == false)
	{
		if (InCannonAnimation == false)
		{
			OnCannonAttackStarted();
			InCannonAnimation = true;
		}
		UseAttackAnimation = true;
	}
	else
	{
		if (InCannonAnimation)
			OnCannonAttackEnded();
		InCannonAnimation = false;
		UseAttackAnimation = false;
	}
	
	if (IsDashAttacking || shootingTimer < (1.0f / currentSpellType->attacksPerSecond))
		return;
	shootingTimer = 0;
	FireEquippedSpell();
}

void AThirdPersonController::StopAttack(const FInputActionValue& Value)
{
	UseAttackAnimation = false;
	InCannonAnimation = false;
	OnCannonAttackEnded();
}

void AThirdPersonController::CounterAttack(const FInputActionValue& Value)
{
	if (selectedAiIndex > -1 && IsDashAttacking == false && AiManager->AiActorsInMap.Num() > 0)
	{
		IsDashAttacking = true;
		attackTimer = 0;
		startingLocation = GetActorLocation();
		//GEngine->AddOnScreenDebugMessage(8079435, 5, FColor::Blue, TEXT("Starting Dash Attack"));
	}
}

void AThirdPersonController::Interact(const FInputActionValue& Value)
{
}

void AThirdPersonController::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AThirdPersonController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AThirdPersonController::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Ongoing, this, &AThirdPersonController::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AThirdPersonController::StopAttack);
		
		EnhancedInputComponent->BindAction(CounterAction, ETriggerEvent::Started, this, &AThirdPersonController::CounterAttack);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThirdPersonController::Interact);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AThirdPersonController::MoveCancelled);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AThirdPersonController::MoveCancelled);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonController::Look);
		
		EnhancedInputComponent->BindAction(Num1Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction1Pressed);
		EnhancedInputComponent->BindAction(Num2Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction2Pressed);
		EnhancedInputComponent->BindAction(Num3Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction3Pressed);
		EnhancedInputComponent->BindAction(Num4Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction4Pressed);
		EnhancedInputComponent->BindAction(Num5Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction5Pressed);
		EnhancedInputComponent->BindAction(Num6Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction6Pressed);
		EnhancedInputComponent->BindAction(Num7Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction7Pressed);
		EnhancedInputComponent->BindAction(Num8Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction8Pressed);
		EnhancedInputComponent->BindAction(Num9Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction9Pressed);
		EnhancedInputComponent->BindAction(Num0Action, ETriggerEvent::Started, this, &AThirdPersonController::InputAction0Pressed);
	}
}

void AThirdPersonController::InputAction1Pressed(const FInputActionValue& value)
{
	currentlySelectedSlot = 0;
	currentSpellType = slotOneProjectile.GetDefaultObject();
}
void AThirdPersonController::InputAction2Pressed(const FInputActionValue& value)
{
	currentlySelectedSlot = 1;
	currentSpellType = slotTwoProjectile.GetDefaultObject();
}
void AThirdPersonController::InputAction3Pressed(const FInputActionValue& value)
{
	currentlySelectedSlot = 2;
	currentSpellType = slotThreeProjectile.GetDefaultObject();
}
void AThirdPersonController::InputAction4Pressed(const FInputActionValue& value)
{
}
void AThirdPersonController::InputAction5Pressed(const FInputActionValue& value)
{
}
void AThirdPersonController::InputAction6Pressed(const FInputActionValue& value)
{
}
void AThirdPersonController::InputAction7Pressed(const FInputActionValue& value)
{
}
void AThirdPersonController::InputAction8Pressed(const FInputActionValue& value)
{
}
void AThirdPersonController::InputAction9Pressed(const FInputActionValue& value)
{
}
void AThirdPersonController::InputAction0Pressed(const FInputActionValue& value)
{
}
