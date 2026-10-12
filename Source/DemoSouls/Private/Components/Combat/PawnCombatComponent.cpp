// Alex Goldberg rights reserved


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h"

/// Registers a spawned weapon to the character's carried weapon map.
/// Optionally, it can also register the weapon as the currently equipped weapon.
void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
	AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	// Perform checks to ensure that the weapon tag is not already registered and that the weapon pointer is valid
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("Weapon with tag %s is already registered in the character's carried weapon map!"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	// Set the weapon in the character's carried weapon map
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
	
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	// If specified, set the weapon tag as the currently equipped weapon tag
	if (bRegisterAsEquippedWeapon)
		CurrentEquippedWeaponTag = InWeaponTagToRegister;

	// const FString WeaponMsg = FString::Printf(TEXT("A weapon named: %s has been registered using the tag %s."), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	// Debug::Print(WeaponMsg);
}

/// Retrieves a weapon from the character's carried weapon map based on the provided FGameplayTag.
/// If the weapon is found, it returns a pointer to the weapon; otherwise, it returns nullptr.
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
			return *FoundWeapon;
	
	return nullptr;
}

/// Retrieves the currently equipped weapon for the character based on the CurrentEquippedWeaponTag.
/// If the tag is valid and the weapon is found, it returns a pointer to the weapon; otherwise, it returns nullptr.
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
		return nullptr;

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

/// Toggles the collision of the weapon based on the specified EToggleDamageType.
/// If the type is CurrentEquippedWeapon, it will toggle the collision of the currently equipped weapon.
/// If it's LeftHand or RightHand, it will toggle the collision of the respective hand's weapon.
void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	else
		ToggleBodyCollisionBoxCollision(bShouldEnable, ToggleDamageType);
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor){}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor){}

/// Toggles the collision of the currently equipped weapon based on the provided boolean value.
void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	if (AWarriorWeaponBase* CurrentEquippedWeapon = GetCharacterCurrentEquippedWeapon())
	{
		check(CurrentEquippedWeapon);

		if (bShouldEnable)
		{
			CurrentEquippedWeapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			CurrentEquippedWeapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
		}
	}
}

void UPawnCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType){}
