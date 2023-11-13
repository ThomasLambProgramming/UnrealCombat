﻿#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	PlayerSkeletalMesh->SetupAttachment(GetRootComponent());
	PlayerSkeletalMesh->RegisterComponent();
	PlayerSkeletalMesh->SetSkeletalMesh(PlayerCharacterMesh);
	PlayerSkeletalMesh->SetRelativeLocation(FVector(0,0,0));
	SetRootComponent(PlayerSkeletalMesh);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,30.0f), FRotator(-50.0f,0.0f,0.0f));
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 6.0f;
	CameraBoom->bDrawDebugLagMarkers = true;
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraMain"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->FieldOfView = 90.0f;

	GEngine->AddOnScreenDebugMessage(
		50,
		15.0f,
		FColor::Yellow,
		FString("Test"));
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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}



void APlayerCharacter::LookInput(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
}
void APlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	GEngine->AddOnScreenDebugMessage(
		50,
		15.0f,
		FColor::Yellow,
		FString::FromInt(MovementVector.X) + FString(":") + FString::FromInt(MovementVector.Y) +
		FString(" Keyboard Value"));
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
void APlayerCharacter::JumpInput(const FInputActionValue& Value)
{
	
}
void APlayerCharacter::StopJumping(const FInputActionValue& Value)
{
	
}
void APlayerCharacter::HotBarInput(int KeyValue)
{
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

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveInput);

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
void APlayerCharacter::Hotbar10Input(const FInputActionValue& Value) {HotBarInput(10);}