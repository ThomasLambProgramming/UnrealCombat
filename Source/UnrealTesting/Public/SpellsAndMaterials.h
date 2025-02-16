#pragma once

#include "CoreMinimal.h"
#include "SpellsAndMaterials.generated.h"

enum SpellTypes
{
	None = 0,
	Fireball,
	BouncingBullet,
	MagicMissile,
	Gun,
};

enum MaterialTypes
{
	DefaultMaterial = 0,
	TrackAlly,
	TrackEnemy,
	DamageIncrease,
	DamageMultiplier,
	VelocityIncrease,
	VelocityMultiplier,
	VelocityAcceleration,
	ReturnAfterDelay,
};
