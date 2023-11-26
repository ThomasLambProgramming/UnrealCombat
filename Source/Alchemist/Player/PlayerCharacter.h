#pragma once
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ALCHEMIST_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCharacter();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	UStaticMesh* SwordMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	UCapsuleComponent* PrimaryCharacterCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	UStaticMesh* ShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* PlayerSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	UStaticMesh* LongbowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	USceneComponent* RootComponentRef;
	
	//What sockets are the sword and shield meshes going to be equipped onto.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	FString SwordSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	FString ShieldSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	float MovementSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	float MaxMovementSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Movement Settings")
	float NoInputDampingAmount = 0.7f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int WeaponState = 0;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//I spent a long time screaming at this function and it not working with delegates till i saw one video explaining you need
	//it to be a ufunction....... lesson learned.
	UFUNCTION()
	void OnHitBoxDetect(AActor* ActorHit, UPrimitiveComponent* OtherComp , const FHitResult& hit);
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
protected:
	FVector2D MovementInput;
	bool HoldingJumpInput = false;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
#pragma region Input
protected:
	/** Called for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called for looking input */
	void LookInput(const FInputActionValue& Value);

	void JumpInput(const FInputActionValue& Value);
	
	void StopJumping(const FInputActionValue& Value);

	void HotBarInput(int KeyValue);
	void Hotbar1Input(const FInputActionValue& Value);
	void Hotbar2Input(const FInputActionValue& Value);
	void Hotbar3Input(const FInputActionValue& Value);
	void Hotbar4Input(const FInputActionValue& Value);
	void Hotbar5Input(const FInputActionValue& Value);
	void Hotbar6Input(const FInputActionValue& Value);
	void Hotbar7Input(const FInputActionValue& Value);
	void Hotbar8Input(const FInputActionValue& Value);
	void Hotbar9Input(const FInputActionValue& Value);
	void Hotbar10Input(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar6;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar7;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar8;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar9;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HotBar10;
#pragma endregion
};
