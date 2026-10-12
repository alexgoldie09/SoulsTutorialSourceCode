// Alex Goldberg rights reserved


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "WarriorGameplayTags.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

/// Retrieves the AWarriorHeroCharacter from the actor info associated with this ability
AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	// If the cached hero character is not valid, attempt to cast the avatar actor to AWarriorHeroCharacter and cache it
	if (!CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	
	return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

/// Retrieves the AWarriorHeroController from the actor info associated with this ability
AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	// If the cached hero controller is not valid, attempt to cast the controller to AWarriorHeroController and cache it
	if (!CachedHeroController.IsValid())
	{
		CachedHeroController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
	}

	return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

/// Retrieves the UHeroCombatComponent from the actor info associated with this ability
UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

/// Creates a GameplayEffectSpecHandle for a hero damage effect with the specified parameters
FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::CreateHeroDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,
	int32 InUsedComboCount)
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

	// Set the SetByCaller magnitude for the base damage using the provided InWeaponBaseDamage value
	EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);

	// Set the SetByCaller magnitude for the current attack type and combo count if the InCurrentAttackTypeTag is valid
	if (InCurrentAttackTypeTag.IsValid())
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);

	return EffectSpecHandle;
}

/// Retrieves the remaining cooldown time for an ability based on the provided cooldown tag
bool UWarriorHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime,
	float& RemainingCooldownTime)
{
	// Validate the InCooldownTag to ensure it is a valid gameplay tag
	check (InCooldownTag.IsValid());

	// Create a GameplayEffectQuery to match any active effects with the specified cooldown tag
	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

	// Retrieve the time remaining and total duration for any active effects that match the cooldown query
	TArray<TPair <float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	// If there are any active effects with the specified cooldown tag, extract the total cooldown time and remaining cooldown time from the first effect in the array
	if (!TimeRemainingAndDuration.IsEmpty())
	{
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
	}

	// Return true if the remaining cooldown time is greater than zero, indicating that the ability is still on cooldown
	return RemainingCooldownTime > 0.f;
}
