// Alex Goldberg rights reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"
/// <summary>
/// Returns the hero's carried weapon that has the specified gameplay tag.
/// </summary>
AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

/// <summary>
/// Returns the hero's currently equipped weapon.
/// </summary>
AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

/// <summary>
/// Returns the damage of the hero's currently equipped weapon at the specified level.
/// </summary>
float UHeroCombatComponent::GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
	return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

/// <summary>
/// Called when the weapon hits a target actor.
/// </summary>
void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// Check if array contains the hit actor, return early if it does
	if (OverlappedActors.Contains(HitActor))
		return;

	// Add the hit actor to the array to prevent multiple hits in a single attack
	OverlappedActors.AddUnique(HitActor);

	// Create and populate the gameplay event data
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	// Send the gameplay event to the actor that was hit
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Shared_Event_MeleeHit,
		EventData);

	// Send a gameplay event to the player to trigger a hit pause effect
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Player_Event_HitPause,
		FGameplayEventData());
}

/// <summary>
/// Called when the weapon is pulled from a target actor.
/// </summary>
void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	// Send a gameplay event to the player to trigger a hit pause effect
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Player_Event_HitPause,
		FGameplayEventData());
}
