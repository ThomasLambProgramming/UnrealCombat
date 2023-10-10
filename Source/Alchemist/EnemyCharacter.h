// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ALCHEMIST_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();


	//Wow unreal really forces that public callable restriction hey.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tweaking Values")
	float m_BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tweaking Values")
	float m_BaseLookUpRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	UBehaviorTree* BehaviorTree;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
