#pragma once

#include "CoreMinimal.h"
#include "ProjectileData.generated.h"

USTRUCT()
struct FProjectileData
{
	GENERATED_BODY()
	enum TrackingState
	{
		None = 0,
		TrackAlly = 1,
		TrackEnemy = 2,
	};
	float Speed = 100;
	float Dropoff = 0;
	FVector ArcingDirection = FVector(0,0,0);
	TrackingState EntityTrackingState = TrackingState::None;
	float Damage = 1;
	float AoeRange = 0;
	int BounceAmount = 0;
};
