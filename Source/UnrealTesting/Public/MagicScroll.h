// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CollisionQueryParams.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicScroll.generated.h"

UCLASS()
class UNREALTESTING_API AMagicScroll : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Settings")
	TEnumAsByte<ECollisionChannel> InitialRaycastChannel;
	
	// Sets default values for this actor's properties
	AMagicScroll();
	AMagicScroll(const FVector& startingLocation);

	FVector originalLocation = FVector(-FLT_MAX);
	FVector originLocation = FVector(-FLT_MAX); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
