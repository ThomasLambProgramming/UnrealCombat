// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIManager.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UENUM()
enum TrackingState
{
	None = 0,
	TrackAlly = 1,
	TrackEnemy = 2,
};

UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectile();
	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AAIManager* AIManager;
public:
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupProjectile(AAIManager* aiManager);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float closestDistanceForTracking = 500;
	float closestDistanceForTrackingSquared;
	AActor* closestEnemyForTracking;
	
	int bounceCount = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	int Multishot = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float attacksPerSecond = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float Speed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float Dropoff = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	FVector ArcingDirection = FVector(0,0,0);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<TrackingState> EntityTrackingState = TrackingState::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float Damage = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float AoeRange = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float trackingForce = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	int BounceAmount = 0;

	bool firstLog = true;
};
