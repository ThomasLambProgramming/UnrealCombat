// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "FirstAiController.generated.h"
/**
 * 
 */
UCLASS()
class ALCHEMIST_API AFirstAiController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	UBehaviorTree* BehaviorTree;
protected:
	void OnPossess(APawn* a_Pawn) override;
};
