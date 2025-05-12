// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LivingEntityBase.h"
#include "GameFramework/Character.h"
#include "BaseAi.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAiHealthChangedSignature);
UCLASS()
class ABaseAi : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseAi();

	//Blueprint call for when the ai is attacked, used for ui updating.
	UFUNCTION(BlueprintImplementableEvent)
	void OnAiHit(AActor* AttackingEnemy, float DamageDone, float currentHealthLeft);

	UFUNCTION(BlueprintPure)
	float GetMaximumHealth();

	virtual void Tick(float DeltaTime) override;

	void TakeDamage(float damageAmount, AActor* damagingActor);
	void FindRandomWanderPoint();
	bool CheckIfPlayerInVision();
	void PersuePlayer();
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(BlueprintReadWrite)
	FLivingEntityBase AiStats;

	AActor* CurrentTarget = nullptr;
	
	UPROPERTY(EditAnywhere)
	bool ShowDebugVisuals = false;
};
