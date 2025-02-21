#include "AIManager.h"

#include "StandardAi.h"
#include "Kismet/GameplayStatics.h"
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

	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStandardAi::StaticClass(), AiActorsInMap);
	for (int i = 0; i < AiActorsInMap.Num(); ++i)
	{
	    AStandardAi* aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[i]);
	    aiAdded->aiManager = this;
	}

	//Spawn 4 base ai agents around the 0,0,0 location 
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	AStandardAi* aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[AiActorsInMap.Num() - 1]);
	aiAdded->aiManager = this;
	
	spawnLocation.X = -500;
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[AiActorsInMap.Num() - 1]);
	aiAdded->aiManager = this;
	
	spawnLocation.X = 0;
	spawnLocation.Y = -500;
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[AiActorsInMap.Num() - 1]);
	aiAdded->aiManager = this;
	
	spawnLocation.Y = 500;
	AiActorsInMap.Add(GetWorld()->SpawnActor<AActor>(DefaultAI, spawnLocation, spawnRotation, defaultParams));
	aiAdded = Cast<AStandardAi, AActor>(AiActorsInMap[AiActorsInMap.Num() - 1]);
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

AActor* AAIManager::FindNearestEnemy(FVector searchLocation, float distanceLimit)
{
	float closestDistance = FLT_MAX;
	int closestIndex = FLT_MIN;
	for (int i = 0; i < AiActorsInMap.Num(); ++i)
	{
		float distance = FVector::DistSquared(AiActorsInMap[i]->GetActorLocation(), searchLocation);
	    if (distance > closestDistance || distance > distanceLimit)
	    	continue;

		closestDistance = distance;
		closestIndex = i;
	}

	if (closestIndex >= 0 && closestIndex <= AiActorsInMap.Num() -1)
		return AiActorsInMap[closestIndex];

	return nullptr;
}

void AAIManager::DamageEnemiesInRadius(FVector searchLocation, float Radius, float damageAmount)
{
	for (int i = 0; i < AiActorsInMap.Num(); ++i)
	{
		float distance = FVector::DistSquared(AiActorsInMap[i]->GetActorLocation(), searchLocation);
	    if (distance < (Radius * Radius))
			Cast<AStandardAi>(AiActorsInMap[i])->DamageAi(damageAmount);
	}

}

