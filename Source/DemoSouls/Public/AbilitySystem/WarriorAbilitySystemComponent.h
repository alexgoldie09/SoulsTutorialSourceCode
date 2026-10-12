// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "WarriorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/// Called when an ability input is pressed, triggering the corresponding ability associated with the provided gameplay tag.
	/// @param InputTag The gameplay tag associated with the ability input that was pressed.
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	
	/// Called when an ability input is released, stopping the corresponding ability associated with the provided gameplay tag.
	/// @param InputTag The gameplay tag associated with the ability input that was released.
	void OnAbilityInputReleased(const FGameplayTag& InputTag);

	/// Grants the hero's default weapon abilities.
	/// @param InDefaultWeaponAbilities The array of default weapon abilities to grant.
	/// @param ApplyLevel The level at which to apply the abilities.
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FWarriorHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	/// Removes the granted weapon abilities from the ability system component.
	/// @param InGrantedAbilitySpecHandlesToRemove The array of granted ability spec handles to remove from the ability system component.
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	void RemoveGrantedWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandlesToRemove);

	/// Tries to activate an ability associated with the provided gameplay tag.
	/// @param AbilityTagToActivate The gameplay tag associated with the ability to activate.
	/// @return True if the ability was successfully activated, false otherwise.
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
