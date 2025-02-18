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
	
	AActor* aiManager = UGameplayStatics::GetActorOfClass(GetWorld(), AAIManager::StaticClass());
	AActor* projectileManager = UGameplayStatics::GetActorOfClass(GetWorld(), AProjectileManager::StaticClass());
	if (aiManager == NULL)
	{
		GEngine->AddOnScreenDebugMessage(4, 10, FColor::Red, TEXT("DID NOT FIND AI MANAGER"));
		return;
	}
	
	ProjectileManager = Cast<AProjectileManager>(projectileManager);
	AiManager = Cast<AAIManager>(aiManager);
	if (AiManager != NULL && AiManager->AiActorsInMap.Num() > 0)
		GEngine->AddOnScreenDebugMessage(4, 10, FColor::Red, TEXT("Found and casted ai manager"));
}

void AThirdPersonController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	shootingTimer += DeltaSeconds;
	if (IsAttacking && selectedAiIndex > -1 && AiManager->AiActorsInMap.Num() > 0)
	{
		attackTimer += GetWorld()->DeltaTimeSeconds;
		FVector currentActorLocation = GetActorLocation();
		if (FVector::Distance(currentActorLocation, AiManager->AiActorsInMap[selectedAiIndex]->GetActorLocation()) < attackLerpingMinDistance)
		{
			IsAttacking = false;
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
				IsAttacking = false;
			}
		}
		return;
	}
	
	FVector cameraForwardDirection = FollowCamera->GetComponentToWorld().GetRotation().GetForwardVector();
	cameraForwardDirection.Z = 0;
	cameraForwardDirection.Normalize();
	FVector cameraRightDirection = FollowCamera->GetComponentToWorld().GetRotation().GetRightVector();
	cameraRightDirection.Z = 0;
	cameraRightDirection.Normalize();

	if (CurrentInputDirection.Length() < 0.1f)
	{
		selectedAiIndex = 0;	
		return;
	}
	
	FVector inputDirectionInWorld = (cameraForwardDirection * CurrentInputDirection.Y + cameraRightDirection * CurrentInputDirection.X);
	inputDirectionInWorld.Normalize();

	if (AiManager->AiActorsInMap.Num() <= 0)
	{
		selectedAiIndex = -1;
		return;
	}
	int closestEnemy = 0;
	//Dot product will always give [-1, 1].
	float previousDot = -2;
    for (int i = 0; i < AiManager->AiActorsInMap.Num(); ++i)
	{
    	if (AiManager->AiActorsInMap[i] == nullptr)
    		continue;
		float dotProduct = inputDirectionInWorld.Dot((AiManager->AiActorsInMap[i]->GetActorLocation() - GetActorLocation()).GetSafeNormal());
		if (dotProduct > previousDot)
		{
			previousDot = dotProduct;
			closestEnemy = i;
		}
	}
	selectedAiIndex = closestEnemy;	
}

void AThirdPersonController::Move(const FInputActionValue& Value)
{
	//Attacking should not be able to be changed once the attack has started.
	if (IsAttacking)
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
	if (IsAttacking || shootingTimer < (1.0f / attackSpeed))
		return;
	shootingTimer = 0;
	
	AProjectile* projectileToFire = ProjectileManager->GetNewProjectile();
	projectileToFire->SetActorLocation(GetActorLocation() + FVector(0,0,50) + GetFollowCamera()->GetForwardVector() * 80);
	projectileToFire->SetActorRotation(GetFollowCamera()->GetForwardVector().ToOrientationRotator());
	projectileToFire->ProjectileMovement->bInterpMovement = false;
	projectileToFire->ProjectileMovement->ResetInterpolation();
	projectileToFire->ProjectileMovement->SetUpdatedComponent(projectileToFire->GetRootComponent());

	FHitResult hitResult;
	FVector raycastEndLocation = GetFollowCamera()->GetComponentLocation() + GetFollowCamera()->GetForwardVector() * 2000;
	FCollisionQueryParams traceParams = FCollisionQueryParams(TEXT("CrosshairTrace"), true, this);
	GetWorld()->LineTraceSingleByChannel(hitResult, GetFollowCamera()->GetComponentLocation(), raycastEndLocation, TraceChannelProperty, traceParams);

	FVector hitLocation;
	if (hitResult.bBlockingHit)
		hitLocation = hitResult.Location;
	else
		hitLocation = raycastEndLocation;
	//DrawDebugSphere(GetWorld(), hitLocation, 20, 10, FColor::Red);

	FVector directionToFire = hitLocation - projectileToFire->GetActorLocation();
	directionToFire.Normalize();
	projectileToFire->ProjectileMovement->Velocity = directionToFire * 5000;
}

void AThirdPersonController::StopAttack(const FInputActionValue& Value)
{
}

void AThirdPersonController::CounterAttack(const FInputActionValue& Value)
{
	if (selectedAiIndex > -1 && IsAttacking == false && AiManager->AiActorsInMap.Num() < 0)
	{
		IsAttacking = true;
		attackTimer = 0;
		startingLocation = GetActorLocation();
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
	}
}

