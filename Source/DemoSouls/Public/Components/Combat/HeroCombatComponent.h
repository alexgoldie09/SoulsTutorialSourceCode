// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "GameplayTagContainer.h"
#include "HeroCombatComponent.generated.h"

class AWarriorHeroWeapon;
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Returns the hero's carried weapon that has the specified gameplay tag.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	/// <summary>
	/// Returns the hero's currently equipped weapon.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	/// <summary>
	/// Returns the damage of the hero's currently equipped weapon at the specified level.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const;

	/// Called when the weapon hits a target actor.
	virtual void OnHitTargetActor(AActor* HitActor) override;

	/// Called when the weapon is pulled from a target actor.
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
