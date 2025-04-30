#include "LivingEntityBase.h"
FLivingEntityBase::FLivingEntityBase()
{
	InitialiseValues();
}

void FLivingEntityBase::CalculateStatValues()
{
	CalculatedMaximumHealth = ValueCalculation(Raw_MaximumHealth, PreMultiplierAdd_MaximumHealth, Multiplier_MaximumHealth, PostMultiplierAdd_MaximumHealth);
	CalculatedMaximumMovementSpeed = ValueCalculation(Raw_MaximumMovementSpeed, PreMultiplierAdd_MaximumMovementSpeed, Multiplier_MaximumMovementSpeed, PostMultiplierAdd_MaximumMovementSpeed);
	CalculatedMovementAcceleration = ValueCalculation(Raw_MovementAcceleration, PreMultiplierAdd_MovementAcceleration, Multiplier_MovementAcceleration, PostMultiplierAdd_MovementAcceleration);
	CalculatedMovementDeceleration = ValueCalculation(Raw_MovementDeceleration, PreMultiplierAdd_MovementDeceleration, Multiplier_MovementDeceleration, PostMultiplierAdd_MovementDeceleration);
	CalculatedDamage = ValueCalculation(Raw_Damage, PreMultiplierAdd_Damage, Multiplier_Damage, PostMultiplierAdd_Damage);
	CalculatedDamageModifier = ValueCalculation(Raw_DamageModifier, PreMultiplierAdd_DamageModifier, Multiplier_DamageModifier, PostMultiplierAdd_DamageModifier);
	CalculatedActionSpeed = ValueCalculation(Raw_ActionSpeed, PreMultiplierAdd_ActionSpeed, Multiplier_ActionSpeed, PostMultiplierAdd_ActionSpeed);
	CalculatedCoolDownRate = ValueCalculation(Raw_CoolDownRate, PreMultiplierAdd_CoolDownRate, Multiplier_CoolDownRate, PostMultiplierAdd_CoolDownRate);
	CalculatedEquipmentSlots = ValueCalculation(Raw_EquipmentSlots, PreMultiplierAdd_EquipmentSlots, Multiplier_EquipmentSlots, PostMultiplierAdd_EquipmentSlots);
	CalculatedSprintSpeed = ValueCalculation(Raw_SprintSpeed, PreMultiplierAdd_SprintSpeed, Multiplier_SprintSpeed, PostMultiplierAdd_SprintSpeed);
	CalculatedHealthRegenPerSecond = ValueCalculation(Raw_HealthRegenPerSecond, PreMultiplierAdd_HealthRegenPerSecond, Multiplier_HealthRegenPerSecond, PostMultiplierAdd_HealthRegenPerSecond);
	CalculatedJumpingPower = ValueCalculation(Raw_JumpingPower, PreMultiplierAdd_JumpingPower, Multiplier_JumpingPower, PostMultiplierAdd_JumpingPower);
	CalculatedMaximumShieldHealth = ValueCalculation(Raw_MaximumShieldHealth, PreMultiplierAdd_MaximumShieldHealth, Multiplier_MaximumShieldHealth, PostMultiplierAdd_MaximumShieldHealth);
	CalculatedAttackSpeed = ValueCalculation(Raw_AttackSpeed, PreMultiplierAdd_AttackSpeed, Multiplier_AttackSpeed, PostMultiplierAdd_AttackSpeed);
	CalculatedCriticalChance = ValueCalculation(Raw_CriticalChance, PreMultiplierAdd_CriticalChance, Multiplier_CriticalChance, PostMultiplierAdd_CriticalChance);
	CalculatedCriticalModifier = ValueCalculation(Raw_CriticalModifier, PreMultiplierAdd_CriticalModifier, Multiplier_CriticalModifier, PostMultiplierAdd_CriticalModifier);
	CalculatedArmour = ValueCalculation(Raw_Armour, PreMultiplierAdd_Armour, Multiplier_Armour, PostMultiplierAdd_Armour);
	CalculatedMaximumMana = ValueCalculation(Raw_MaximumMana, PreMultiplierAdd_MaximumMana, Multiplier_MaximumMana, PostMultiplierAdd_MaximumMana);
	CalculatedManaRegenSpeed = ValueCalculation(Raw_ManaRegenSpeed, PreMultiplierAdd_ManaRegenSpeed, Multiplier_ManaRegenSpeed, PostMultiplierAdd_ManaRegenSpeed);
	CalculatedBlockAttackChance = ValueCalculation(Raw_BlockAttackChance, PreMultiplierAdd_BlockAttackChance, Multiplier_BlockAttackChance, PostMultiplierAdd_BlockAttackChance);
	CalculatedLuck = ValueCalculation(Raw_Luck, PreMultiplierAdd_Luck, Multiplier_Luck, PostMultiplierAdd_Luck);
	CalculatedProjectileSpeedModifier = ValueCalculation(Raw_ProjectileSpeedModifier, PreMultiplierAdd_ProjectileSpeedModifier, Multiplier_ProjectileSpeedModifier, PostMultiplierAdd_ProjectileSpeedModifier);
}

//Put calculation into separate function as reordering / changing or adding values is easier to add a parameter
float FLivingEntityBase::ValueCalculation(float RawValue, float PreMultiplierAdd, float ValueMultiplier, float PostMultiplierAdd)
{
	return (RawValue + PreMultiplierAdd) * ValueMultiplier + PostMultiplierAdd;
}
int FLivingEntityBase::ValueCalculation(int RawValue, int PreMultiplierAdd, int ValueMultiplier, int PostMultiplierAdd)
{
	return (RawValue + PreMultiplierAdd) * ValueMultiplier + PostMultiplierAdd;
}

//Separate init over calculating stat values since values like current health should not be modified
void FLivingEntityBase::InitialiseValues()
{
	CalculateStatValues();
	CurrentHealth = CalculatedMaximumHealth;
}
