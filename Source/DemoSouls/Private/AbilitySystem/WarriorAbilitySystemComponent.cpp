// Alex Goldberg rights reserved


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

#include "InputTriggers.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

/// Called when an ability input is pressed, triggering the corresponding ability associated with the provided gameplay tag.
/// @param InputTag The gameplay tag associated with the ability input that was pressed.
void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	// If input tag is not valid, return early
	if (!InputTag.IsValid())
		return;

	// Loop through all activatable abilities and try to activate the ability that matches the input tag
	for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If the ability does not have the input tag, skip it
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) continue;

		// If the ability is toggleable, check if it's active and either cancel or activate it accordingly
		if (InputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
			// If the ability is active and has the input tag, cancel it
			CancelAbilityHandle(AbilitySpec.Handle);
		else
			// Try to activate the ability regularly
			TryActivateAbility(AbilitySpec.Handle);
	}
}

/// Called when an ability input is released, stopping the corresponding ability associated with the provided gameplay tag.
/// @param InputTag The gameplay tag associated with the ability input that was released.
void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid() || !InputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
		return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			// If the ability is active and has the input tag, cancel it
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

/// Grants the hero's default weapon abilities.
/// @param InDefaultWeaponAbilities The array of default weapon abilities to grant.
/// @param InSpecialWeaponAbilities The array of special weapon abilities to grant.
/// @param ApplyLevel The level at which to apply the abilities.
void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(
	const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FWarriorHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty() && InSpecialWeaponAbilities.IsEmpty())
		return;

	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}

	for (const FWarriorHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

/// Removes the granted weapon abilities from the ability system component.
/// @param InGrantedAbilitySpecHandlesToRemove The array of granted ability spec handles to remove from the ability system component.
void UWarriorAbilitySystemComponent::RemoveGrantedWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandlesToRemove)
{
	if (InGrantedAbilitySpecHandlesToRemove.IsEmpty())
		return;

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : InGrantedAbilitySpecHandlesToRemove)
	{
		if (AbilitySpecHandle.IsValid())
			ClearAbility(AbilitySpecHandle);
	}

	InGrantedAbilitySpecHandlesToRemove.Empty();
}

/// Tries to activate an ability associated with the provided gameplay tag.
/// @param AbilityTagToActivate The gameplay tag associated with the ability to activate.
/// @return True if the ability was successfully activated, false otherwise.
bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid())

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(),FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	return false;
}
