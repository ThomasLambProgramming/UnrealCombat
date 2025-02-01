// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"

class AStandardAi;

UCLASS()
class AAIManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= AI, meta = (AllowPrivateAccess = true))
	TSubclassOf<ACharacter> DefaultAI;

public:	
	// Sets default values for this actor's properties
	AAIManager();

	TArray<AActor*> AiActorsInMap;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
