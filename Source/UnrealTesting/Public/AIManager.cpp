#include "AIManager.h"

#include "StandardAi.h"
#include "Tests/AutomationCommon.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();

	FVector spawnLocation = FVector(500,0,96);
	FRotator spawnRotation = FRotator(0,0,0);
	FActorSpawnParameters defaultParams;
	
	AActor* aiSpawned = GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams);
	spawnLocation.X = -500;
	AActor* aiSpawned1 = GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams);
	spawnLocation.X = 0;
	spawnLocation.Y = -500;
	AActor* aiSpawned2 = GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams);
	spawnLocation.Y = 500;
	AActor* aiSpawned3 = GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams);

	
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

