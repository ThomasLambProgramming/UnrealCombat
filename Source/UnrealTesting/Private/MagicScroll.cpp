// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicScroll.h"


// Sets default values
AMagicScroll::AMagicScroll()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
AMagicScroll::AMagicScroll(const FVector& startingLocation)
{
	AMagicScroll();
	originalLocation = startingLocation;
}

// Called when the game starts or when spawned
void AMagicScroll::BeginPlay()
{
	Super::BeginPlay();

	FHitResult result;
	FVector checkLocation = originalLocation + FVector(0, 500, 0);
	FVector endLocation = (originalLocation - checkLocation);
	endLocation.Normalize();
	endLocation *= 1000;
	if (GetWorld()->LineTraceSingleByChannel(result, checkLocation, endLocation, InitialRaycastChannel))
	{
		originLocation = result.Location;	
	}
}

// Called every frame
void AMagicScroll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

