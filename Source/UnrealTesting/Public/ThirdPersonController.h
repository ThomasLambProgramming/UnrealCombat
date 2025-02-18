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

	//COMPONENTS-------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	
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
	AThirdPersonController();
	void Tick(float DeltaSeconds) override;
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	void Move(const FInputActionValue& Value);
	void MoveCancelled(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void Attack(const FInputActionValue& Value);
	void StopAttack(const FInputActionValue& Value);
	void CounterAttack(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

	void BeginPlay() override;
	bool FindClosestEnemy();
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AAIManager* AiManager;
	AProjectileManager* ProjectileManager;
	bool IsAttacking = false;
	float attackTimer = 0;
	FVector startingLocation;
	float shootingTimer = 0;
	FVector2D CurrentInputDirection = FVector2D(0,0);
	int selectedAiIndex = -1;
};


