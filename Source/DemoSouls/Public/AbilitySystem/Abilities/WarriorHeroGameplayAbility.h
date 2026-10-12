// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class AWarriorHeroCharacter;
class AWarriorHeroController;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	/// Retrieves the AWarriorHeroCharacter from the actor info associated with this ability
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();

	/// Retrieves the AWarriorHeroController from the actor info associated with this ability
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroController* GetHeroControllerFromActorInfo();

	/// Retrieves the UHeroCombatComponent from the actor info associated with this ability
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	/// Creates a GameplayEffectSpecHandle for a hero damage effect with the specified parameters
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle CreateHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

	/// Retrieves the remaining cooldown time for an ability based on the provided cooldown tag
	/// @param InCooldownTag - The gameplay tag associated with the cooldown to check
	/// @param TotalCooldownTime - Output parameter that will hold the total cooldown time for the ability
	/// @param RemainingCooldownTime - Output parameter that will hold the remaining cooldown time for the ability
	/// @return True if the ability has a cooldown and the times were successfully retrieved
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);

private:
	/// Store a weak pointer to the hero character to avoid repeated casts and improve performance
	TWeakObjectPtr<AWarriorHeroCharacter> CachedHeroCharacter;
	/// Store a weak pointer to the hero controller to avoid repeated casts and improve performance
	TWeakObjectPtr<AWarriorHeroController> CachedHeroController;
};
