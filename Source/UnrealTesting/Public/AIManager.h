// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAi.h"
#include "AIManager.generated.h"

UCLASS()
class AAIManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= AI, meta = (AllowPrivateAccess = true))
	TSubclassOf<ACharacter> DefaultAI;

public:
	static AAIManager* AIManagerInstance;

	// Sets default values for this actor's properties
	AAIManager();

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> AiActorsInMap;

	AActor* PlayerCharacter;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Searches array for the ai.
	void DeleteAi(ABaseAi* AiToFind);

	
	//Deleted ai from that index without searching.
	UFUNCTION(Exec, Category="Commands")
	void DeleteAi(int index);

	AActor* FindNearestEnemy(FVector searchLocation, float distanceLimit);
	void DamageEnemiesInRadius(FVector searchLocation, float Radius, float damageAmount, AActor* damagingActor);
};

AAIManager* AAIManager::AIManagerInstance = nullptr;

