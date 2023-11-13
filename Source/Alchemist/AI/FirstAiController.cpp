// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyCharacter.h"

void AFirstAiController::OnPossess(APawn* a_Pawn)
{
	//Run base method.
	Super::OnPossess(a_Pawn);

	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(a_Pawn);
	if (enemyCharacter != nullptr)
	{
		UBehaviorTree* behTree = enemyCharacter->BehaviorTree;
		if (behTree != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Ran the behaviour tree logic"));
			RunBehaviorTree(behTree);
		}
	}
}