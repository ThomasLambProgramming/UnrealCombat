// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileManager.h"


// Sets default values
AProjectileManager::AProjectileManager()
{
	MaxProjectilesInScene = 200;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	projectileIndex = 0;
	defaultProjectileRotation = FRotator(0,0,0);
	defaultProjectileLocation = FVector(0,0,-1000);
}

void AProjectileManager::ResetProjectile(int a_index)
{
	currentProjectilesInScene[a_index]->SetActorLocation(defaultProjectileLocation);
	currentProjectilesInScene[a_index]->SetActorRotation(defaultProjectileRotation);
	currentProjectilesInScene[a_index]->ResetProjectile();
}
AProjectile* AProjectileManager::GetNewProjectile()
{
	int currentIndex = projectileIndex;

	if (projectileIndex < MaxProjectilesInScene - 1)
		projectileIndex++;
	else
		projectileIndex = 0;
		
	//Spawn new projectile and init its stats.
	ResetProjectile(currentIndex);
	return currentProjectilesInScene[currentIndex];
}
// Called when the game starts or when spawned
void AProjectileManager::BeginPlay()
{
	Super::BeginPlay();
	//currentProjectilesInScene.Init()
	//Pre init all projectiles so it doesnt generate anything once the game is loaded.
	for (int i = 0; i < MaxProjectilesInScene; i++)
	{
		currentProjectilesInScene.Add(GetWorld()->SpawnActor<AProjectile>(defaultProjectile, defaultProjectileLocation, defaultProjectileRotation, defaultProjectileSpawnParams));
	}
}
// Called every frame
void AProjectileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

