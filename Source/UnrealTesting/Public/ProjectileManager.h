// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "ProjectileManager.generated.h"

UCLASS()
class UNREALTESTING_API AProjectileManager : public AActor
{
	GENERATED_BODY()

public:
	AProjectileManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= AI, meta = (AllowPrivateAccess = true))
	int MaxProjectilesInScene;
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AProjectile> defaultProjectile;
	
	int projectileIndex = 0;
	TArray<AProjectile*> currentProjectilesInScene;

	FRotator defaultProjectileRotation;
	FVector defaultProjectileLocation;
	FActorSpawnParameters defaultProjectileSpawnParams;
	
	AProjectile* GetNewProjectile();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ResetProjectile(int a_index);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
