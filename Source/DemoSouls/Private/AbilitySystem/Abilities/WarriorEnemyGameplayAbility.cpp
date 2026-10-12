// Alex Goldberg rights reserved


#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"

#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Characters/WarriorEnemyCharacter.h"

/// <summary>
/// Retrieves the AWarriorEnemyCharacter instance from the ability's actor info.
/// If the cached pointer is invalid, it attempts to cast the AvatarActor from the CurrentActorInfo to AWarriorEnemyCharacter and caches it for future use.
/// </summary>
AWarriorEnemyCharacter* UWarriorEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedWarriorEnemyCharacter.IsValid())
		CachedWarriorEnemyCharacter = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
	
	return CachedWarriorEnemyCharacter.IsValid() ? CachedWarriorEnemyCharacter.Get() : nullptr;
}

/// <summary>
/// Retrieves the UEnemyCombatComponent instance from the ability's actor info.
/// It first gets the AWarriorEnemyCharacter instance and then calls GetEnemyCombatComponent() on it to retrieve the combat component.
/// </summary>
UEnemyCombatComponent* UWarriorEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UWarriorEnemyGameplayAbility::CreateEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	// Validate the EffectClass to ensure it is a valid GameplayEffect class
	check(EffectClass);

	// Create a GameplayEffectContextHandle to provide context for the effect
	FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// Create a GameplayEffectSpecHandle using the provided EffectClass, ability level, and context handle
	FGameplayEffectSpecHandle EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
		);

	// Set the SetByCaller magnitude for the base damage using the provided InDamageScalableFloat value
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		WarriorGameplayTags::Shared_SetByCaller_BaseDamage,
	InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));

	return EffectSpecHandle;
	
}
