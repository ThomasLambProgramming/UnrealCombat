#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UniversalRpgElements/Public/AttackHitbox.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* RootComponentReference = CreateDefaultSubobject<USceneComponent>("RootComponent");
	//SetRootComponent(RootComponentReference);

	PrimaryCharacterCollider = CreateDefaultSubobject<UCapsuleComponent>("CharacterCollider");
	PrimaryCharacterCollider->SetupAttachment(GetRootComponent());
	SetRootComponent(PrimaryCharacterCollider);
	
	PlayerSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	PlayerSkeletalMesh->SetupAttachment(PrimaryCharacterCollider);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(PlayerSkeletalMesh);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 6.0f;
	CameraBoom->bDrawDebugLagMarkers = true;
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraMain"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 
	FollowCamera->FieldOfView = 90.0f;
	
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TArray<AActor*> actorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAttackHitbox::StaticClass(), actorsToFind);
	for (int i = 0; i < actorsToFind.Num(); i++)
	{
		AAttackHitbox* hitbox = Cast<AAttackHitbox>(actorsToFind[i]);
		if (hitbox)
		{
			hitbox->OnHitDelegate.AddUniqueDynamic(this, &APlayerCharacter::OnHitBoxDetect);
		}
	}
}

void APlayerCharacter::OnHitBoxDetect(AActor* ActorHit, UPrimitiveComponent* OtherComp , const FHitResult& hit)
{
	GEngine->AddOnScreenDebugMessage(20, 2, FColor::Emerald, ActorHit->GetName());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(50, 15.0f, FColor::Yellow, MovementInput.ToString() + FString(" Keyboard Value"));	
	if (MovementInput != FVector2D::Zero())
	{
		PrimaryCharacterCollider->AddForce(
			GetActorForwardVector() * MovementInput.Y * MovementSpeed +
			GetActorRightVector() * MovementInput.X * MovementSpeed);
		
		if (PrimaryCharacterCollider->GetComponentVelocity().Length() > MaxMovementSpeed)
		{
			PrimaryCharacterCollider->SetPhysicsLinearVelocity(
				PrimaryCharacterCollider->GetComponentVelocity().GetSafeNormal() * MaxMovementSpeed);
		}
	}
	else if (PrimaryCharacterCollider->GetComponentVelocity().SquaredLength() > 0.01f)
	{
		//Multiply the current velocity by 0.7 or etc to rapidly slowdown the users velocity.
		//Dont affect the z as then it would affect the users falling velocity and etc.
		FVector currentVelocity = PrimaryCharacterCollider->GetComponentVelocity();
		float ZAmount = currentVelocity.Z;
		currentVelocity.Z = 0;
		currentVelocity *= NoInputDampingAmount;

		if (currentVelocity.SquaredLength() < 0.01f)
			currentVelocity = FVector::Zero();
		
		currentVelocity.Z = ZAmount;
		PrimaryCharacterCollider->SetPhysicsLinearVelocity(currentVelocity);
	}

	
}



void APlayerCharacter::LookInput(const FInputActionValue& Value)
{
	// input is a Vector2D
	//FVector2D LookAxisVector = Value.Get<FVector2D>();
	
}

void APlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementInput = MovementVector;
	GEngine->AddOnScreenDebugMessage(49, 15.0f, FColor::Yellow, MovementInput.ToString() + FString("Input"));	
}

void APlayerCharacter::JumpInput(const FInputActionValue& Value)
{
	
}

void APlayerCharacter::StopJumping(const FInputActionValue& Value)
{
	
}

void APlayerCharacter::HotBarInput(int KeyValue)
{
	WeaponState = KeyValue;
	GEngine->AddOnScreenDebugMessage(50, 15.0f, FColor::Yellow, FString::FromInt(KeyValue) + FString(" Keyboard Value"));	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);

		//Moving, function only gets the value and will be consumed later so all events can use the one function.
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::MoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &APlayerCharacter::MoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Ongoing, this, &APlayerCharacter::MoveInput);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookInput);

		EnhancedInputComponent->BindAction(HotBar1, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar1Input);
		EnhancedInputComponent->BindAction(HotBar2, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar2Input);
		EnhancedInputComponent->BindAction(HotBar3, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar3Input);
		EnhancedInputComponent->BindAction(HotBar4, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar4Input);
		EnhancedInputComponent->BindAction(HotBar5, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar5Input);
		EnhancedInputComponent->BindAction(HotBar6, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar6Input);
		EnhancedInputComponent->BindAction(HotBar7, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar7Input);
		EnhancedInputComponent->BindAction(HotBar8, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar8Input);
		EnhancedInputComponent->BindAction(HotBar9, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar9Input);
		EnhancedInputComponent->BindAction(HotBar10, ETriggerEvent::Triggered, this, &APlayerCharacter::Hotbar10Input);
	}
}

void APlayerCharacter::Hotbar1Input(const FInputActionValue& Value) {HotBarInput(1);}
void APlayerCharacter::Hotbar2Input(const FInputActionValue& Value) {HotBarInput(2);}  
void APlayerCharacter::Hotbar3Input(const FInputActionValue& Value) {HotBarInput(3);}
void APlayerCharacter::Hotbar4Input(const FInputActionValue& Value) {HotBarInput(4);}
void APlayerCharacter::Hotbar5Input(const FInputActionValue& Value) {HotBarInput(5);}
void APlayerCharacter::Hotbar6Input(const FInputActionValue& Value) {HotBarInput(6);}
void APlayerCharacter::Hotbar7Input(const FInputActionValue& Value) {HotBarInput(7);}
void APlayerCharacter::Hotbar8Input(const FInputActionValue& Value) {HotBarInput(8);}
void APlayerCharacter::Hotbar9Input(const FInputActionValue& Value) {HotBarInput(9);}
void APlayerCharacter::Hotbar10Input(const FInputActionValue& Value) {HotBarInput(0);}
