// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "WarriorHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DEMOSOULS_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	/// <summary>
	/// The weapon's linked anim layer class to use when this weapon is equipped.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWarriorHeroWeaponData HeroWeaponData;

	/// <summary>
	/// Assigns the granted ability spec handles to the weapon.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
private:
	/// <summary>
	/// The granted ability spec handles for the weapon's abilities.
	/// </summary>
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
