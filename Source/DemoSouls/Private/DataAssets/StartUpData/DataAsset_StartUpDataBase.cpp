// Alex Goldberg rights reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

/// Give the abilities to the ability system component
void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	// Perform a null check on the ability system component
	check(InASCToGive);

	// Grant the abilities to the ability system component
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	// Check if the startup gameplay effects array is empty
	if (!StartUpGameplayEffects.IsEmpty())
	{
		// Loop through the startup gameplay effects array and apply each effect to the ability system component
		for (const TSubclassOf<UGameplayEffect>& Effect : StartUpGameplayEffects)
		{
			if (!Effect) continue;

			UGameplayEffect* EffectCDO = Effect->GetDefaultObject<UGameplayEffect>();

			InASCToGive->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}
}

/// Grant the abilities to the ability system component
void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& AbilitiesToGive,
	UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// Check if the abilities array is empty
	if (AbilitiesToGive.IsEmpty())
		return;

	// Loop through the abilities array and give each ability to the ability system component
	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : AbilitiesToGive)
	{
		// If not an ability, continue to the next ability
		if (!Ability) continue;

		// Create a new ability spec and bind it to the ability system component
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		// Give the ability to the ability system component
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
