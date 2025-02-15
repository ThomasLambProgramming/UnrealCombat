#pragma once

#include "CoreMinimal.h"
#include "ProjectileManager.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ThirdPersonController.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AAIManager;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)

class AThirdPersonController : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CounterAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
	
	FVector2D CurrentInputDirection = FVector2D(0,0);
	FVector cameraForwardDirection = FVector(1,0,0);
	FVector cameraRightDirection = FVector(0,1,0);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float maximumDistanceForEnemyHit = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* powerCurveOnHit;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float attackDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float attackLerpingMinDistance = 100;

	//For some reason the lerping when attacking lowers the player character by a very small amount.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	FVector attackVerticalOffset= FVector(0,0,10);

	int selectedAiIndex = -1;
	
public:
	AThirdPersonController();
	void Tick(float DeltaSeconds) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void MoveCancelled(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void Attack(const FInputActionValue& Value);
	void StopAttack(const FInputActionValue& Value);
	void CounterAttack(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

protected:

	void BeginPlay() override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AAIManager* AiManager;
	AProjectileManager* ProjectileManager;
	bool IsAttacking = false;
	float attackTimer = 0;
	FVector startingLocation;
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


