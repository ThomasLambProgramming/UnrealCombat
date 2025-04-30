// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAi.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAiHealthChangedSignature);
UCLASS()
class ABaseAi : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAi();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float MaxHealth= 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= State)
	bool IsDead = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGetHit(AActor* attackingEnemy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void RegisterDamageToUI();

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnAiHealthChangedSignature OnAiHealthChangedDelegate;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DamageAi(float damageAmount, AActor* damagingActor);
	void KillEnemyInstantly();
};
