#pragma once

#include "LivingEntityBase.generated.h"
//We assume that all creatures will have the same base so working with the game will be easier.
//All living entities will take off this. Hopefully doing it this way will allow me to easily add things later like
//enemy's being able to inherit items for wacky game modes without needing to rework them.

//Class names are to be verbose by default as there is going to probably be an insane amount of variables / changes
USTRUCT()
struct FLivingEntityBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CurrentHealth = 0;

	FLivingEntityBase();
	void CalculateStatValues();
	float ValueCalculation(float RawValue, float PreMultiplierAdd, float ValueMultiplier, float PostMultiplierAdd);
	int ValueCalculation(int RawValue, int PreMultiplierAdd, int ValueMultiplier, int PostMultiplierAdd);
	void InitialiseValues();
	
	//Raw specified statistics are the base values. Do not edit them during runtime.
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_MaximumHealth = 100;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_MaximumMovementSpeed = 400;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_MovementAcceleration = 60.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_MovementDeceleration = 60.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_Damage = 5.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_DamageModifier = 1.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_ActionSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_CoolDownRate = 1.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	int Raw_EquipmentSlots = 1;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_SprintSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	uint64 Raw_ExperiencePoints;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_HealthRegenPerSecond = 0.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_JumpingPower = 100.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	int   Raw_MaxJumpCount = 1;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_MaximumShieldHealth = 0.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_AttackSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_CriticalChance = 5.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_CriticalModifier = 1.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_Armour = 10;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_MaximumMana = 100.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_ManaRegenSpeed = 10.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_BlockAttackChance = 0.0f;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_Luck = 1;
	UPROPERTY(EditAnywhere, Category="Raw Stats")
	float Raw_ProjectileSpeedModifier = 1.0f;

	
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_MaximumHealth = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_MaximumMovementSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_MovementAcceleration = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_MovementDeceleration = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_Damage = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_DamageModifier = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_ActionSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_CoolDownRate = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	int   PreMultiplierAdd_EquipmentSlots = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_SprintSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_HealthRegenPerSecond = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_JumpingPower = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_MaximumShieldHealth = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_AttackSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_CriticalChance = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_CriticalModifier = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_Armour = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_MaximumMana = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_ManaRegenSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_BlockAttackChance = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_Luck = 0;
	UPROPERTY(EditAnywhere, Category="PreMultiplier Stats")
	float PreMultiplierAdd_ProjectileSpeedModifier = 0;

	
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_MaximumHealth = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_MaximumMovementSpeed = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_MovementAcceleration = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_MovementDeceleration = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_Damage = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_DamageModifier = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_ActionSpeed = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_CoolDownRate = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	int   Multiplier_EquipmentSlots = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_SprintSpeed = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_HealthRegenPerSecond = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_JumpingPower = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_MaximumShieldHealth = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_AttackSpeed = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_CriticalChance = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_CriticalModifier = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_Armour = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_MaximumMana = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_ManaRegenSpeed = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_BlockAttackChance = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_Luck = 1;
	UPROPERTY(EditAnywhere, Category="Multiplier Stats")
	float Multiplier_ProjectileSpeedModifier = 1;


	
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_MaximumHealth = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_MaximumMovementSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_MovementAcceleration = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_MovementDeceleration = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_Damage = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_DamageModifier = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_ActionSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_CoolDownRate = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	int	  PostMultiplierAdd_EquipmentSlots = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_SprintSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_HealthRegenPerSecond = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_JumpingPower = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_MaximumShieldHealth = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_AttackSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_CriticalChance = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_CriticalModifier = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_Armour = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_MaximumMana = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_ManaRegenSpeed = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_BlockAttackChance = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_Luck = 0;
	UPROPERTY(EditAnywhere, Category="PostMultiplier Stats")
	float PostMultiplierAdd_ProjectileSpeedModifier = 0;

	
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedMaximumHealth = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedMaximumMovementSpeed = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedMovementAcceleration = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedMovementDeceleration = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedDamage = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedDamageModifier = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedActionSpeed = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedCoolDownRate = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	int	  CalculatedEquipmentSlots = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedSprintSpeed = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedHealthRegenPerSecond = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedJumpingPower = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedMaximumShieldHealth = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedAttackSpeed = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedCriticalChance = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedCriticalModifier = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedArmour = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedMaximumMana = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedManaRegenSpeed = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedBlockAttackChance = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedLuck = 0;
	UPROPERTY(EditAnywhere, Category="Calculated Stats")
	float CalculatedProjectileSpeedModifier = 0;
};
