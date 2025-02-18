#include "AIManager.h"

#include "StandardAi.h"
#include "Tests/AutomationCommon.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AiActorsInMap = TArray<AActor*>();
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();

	FVector spawnLocation = FVector(500,0,96);
	FRotator spawnRotation = FRotator(0,0,0);
	FActorSpawnParameters defaultParams;

	//Spawn 4 base ai agents around the 0,0,0 location 
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	AStandardAi* aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[0]);
	aiAdded->aiManager = this;
	
	spawnLocation.X = -500;
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[1]);
	aiAdded->aiManager = this;
	
	spawnLocation.X = 0;
	spawnLocation.Y = -500;
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[2]);
	aiAdded->aiManager = this;
	
	spawnLocation.Y = 500;
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[3]);
	aiAdded->aiManager = this;
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIManager::DeleteAi(AStandardAi* index)
{
	int previousCount= AiActorsInMap.Num();

	for (int i = 0; i < AiActorsInMap.Num(); i++)
	{
		if (AiActorsInMap[i] == index)
		{
			AiActorsInMap[i]->Destroy();
			AiActorsInMap.RemoveAt(i, 1, EAllowShrinking::Yes);
			GEngine->AddOnScreenDebugMessage(24368085, 5, FColor::Blue, TEXT("FOUND AI AND DELETED"));
		}
	}
	
	int currentCount = AiActorsInMap.Num();

	GEngine->AddOnScreenDebugMessage(24368079, 5, FColor::Blue, TEXT("AICOUNTONDELETE") + FString::FromInt(previousCount) + " " + FString::FromInt(currentCount) );
}
void AAIManager::DeleteAi(int index)
{
	int previousCount= AiActorsInMap.Num();
    AiActorsInMap[index]->Destroy();
    AiActorsInMap.RemoveAt(index, 1, EAllowShrinking::Yes);
    GEngine->AddOnScreenDebugMessage(24368085, 5, FColor::Blue, TEXT("FOUND AI AND DELETED"));
	int currentCount = AiActorsInMap.Num();
	GEngine->AddOnScreenDebugMessage(24368079, 5, FColor::Blue, TEXT("AICOUNTONDELETE") + FString::FromInt(previousCount) + " " + FString::FromInt(currentCount) );
}

