// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackHitbox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHitDelegate, AActor*, ActorHit, UPrimitiveComponent*, OtherComp, const FHitResult&, hit);

UCLASS()
class UNIVERSALRPGELEMENTS_API AAttackHitbox : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* HitboxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> TargetsHit;

	UPROPERTY(BlueprintAssignable)
	FOnHitDelegate OnHitDelegate;
	
public:
	// Sets default values for this actor's properties
	AAttackHitbox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitboxCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
