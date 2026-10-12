// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"

class AWarriorWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	/// Registers a spawned weapon to the character's carried weapon map. Optionally, it can also register the weapon as the currently equipped weapon.
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	/// Gets a carried weapon for the character based on the FGameplayTag.
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	/// Gets the currently equipped weapon for the character
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	/// Toggles the collision of the weapon based on the specified EToggleDamageType. If the type is CurrentEquippedWeapon, it will toggle the collision of the currently equipped weapon. If it's LeftHand or RightHand, it will toggle the collision of the respective hand's weapon.
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	/// Called when the weapon hits a target actor.
	/// This function can be overridden in derived classes to implement custom behavior when a weapon hits a target.
	virtual void OnHitTargetActor(AActor* HitActor);

	/// Called when the weapon is pulled from a target actor.
	/// This function can be overridden in derived classes to implement custom behavior when a weapon is pulled from a target.
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);
	
	/// The tag for the current equipped weapon
	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

protected:
	/// Toggles the collision of the currently equipped weapon based on the specified boolean value.
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	/// Toggles the collision of the body collision boxes (left and right hand) based on the specified boolean value and EToggleDamageType.
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);
	
	/// A list of actors that have been overlapped by the weapon.
	/// This is used to keep track of which actors have been hit by the weapon during an attack, preventing multiple hits on the same actor in a single attack sequence.
	TArray<AActor*> OverlappedActors;
	
private:
	/// A map that associates FGameplayTags with the weapons carried by the character. This allows for easy retrieval of weapons based on their tags.
	TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeaponMap;
};
