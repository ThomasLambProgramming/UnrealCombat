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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWandChangedSignature, int32, WandID);

UCLASS(config=Game)
class AThirdPersonController : public ACharacter
{
	GENERATED_BODY()

	//COMPONENTS-------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float maximumDistanceForEnemyHit = 300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float attackSpeed = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* powerCurveOnHit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float attackDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float attackLerpingMaxDistance = 100;
	float attackLerpingMaxDistanceSquared = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float attackLerpingMinDistance = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attacking, meta = (AllowPrivateAccess = "true"))
	float MinimumAllowedDashDotProduct = 0.8f;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWandChanged(const int& WandID);
	
	UFUNCTION(BlueprintCallable)
	void OnWandChangedUI(const int& WandID);

	UPROPERTY(BlueprintAssignable)
	FOnWandChangedSignature TestingDelegate;
	
	
	AThirdPersonController();
	void Tick(float DeltaSeconds) override;
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	void Move(const FInputActionValue& Value);
	void MoveCancelled(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void FireEquippedSpell();

	void Attack(const FInputActionValue& Value);
	void StopAttack(const FInputActionValue& Value);
	void CounterAttack(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

	void BeginPlay() override;
	bool FindClosestEnemy();
	void ProcessLerpDash();
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AAIManager* AiManager;
	//AProjectileManager* ProjectileManager;
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AProjectile> slotOneProjectile;
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AProjectile> slotTwoProjectile;
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AProjectile> slotThreeProjectile;

	int currentlySelectedSlot = 0;
	AProjectile* currentSpellType;
	
	bool IsDashAttacking = false;
	float attackTimer = 0;
	FVector startingLocation;
	float shootingTimer = 0;
	FVector2D CurrentInputDirection = FVector2D(0,0);
	int selectedAiIndex = -1;

	
	void InputAction1Pressed(const FInputActionValue& value);
	void InputAction2Pressed(const FInputActionValue& value);
	void InputAction3Pressed(const FInputActionValue& value);
	void InputAction4Pressed(const FInputActionValue& value);
	void InputAction5Pressed(const FInputActionValue& value);
	void InputAction6Pressed(const FInputActionValue& value);
	void InputAction7Pressed(const FInputActionValue& value);
	void InputAction8Pressed(const FInputActionValue& value);
	void InputAction9Pressed(const FInputActionValue& value);
	void InputAction0Pressed(const FInputActionValue& value);
	
	//INPUT--------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CounterAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num4Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num5Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num6Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num7Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num8Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num9Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Num0Action;
};


