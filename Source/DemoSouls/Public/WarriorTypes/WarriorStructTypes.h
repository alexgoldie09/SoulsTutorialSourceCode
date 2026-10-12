// Alex Goldberg all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorStructTypes.generated.h"

class UWarriorHeroLinkedAnimLayer;
class UWarriorHeroGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FWarriorHeroAbilitySet
{
	GENERATED_BODY()

	/// <summary>
	/// The input tag that will be used to trigger the ability
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	/// <summary>
	/// The ability that will be granted to the hero when the input tag is triggered
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroGameplayAbility> AbilityToGrant;
	
	/// <summary>
	/// Check if the ability set is valid
	/// </summary>
	bool IsValid() const;
};

/// <summary>
/// A struct that represents a special ability set for a warrior hero.
/// Inherits from FWarriorHeroAbilitySet and adds additional properties for special abilities.
/// </summary>
USTRUCT(BlueprintType)
struct FWarriorHeroSpecialAbilitySet : public FWarriorHeroAbilitySet
{
	GENERATED_BODY()

	/// <summary>
	/// The soft reference to the material that will be used for the ability icon in the UI
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> SoftAbilityIconMaterial;

	/// <summary>
	/// The gameplay tag that will be used to identify the ability's cooldown state
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Player.Cooldown"))
	FGameplayTag AbilityCooldownTag;
};

USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
	GENERATED_BODY()

	/// <summary>
	/// The weapon's linked anim layer class to use when this weapon is equipped.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroLinkedAnimLayer> WeaponAnimLayerToLink;

	/// <summary>
	/// The input mapping context that will be added to the hero's input component when this weapon is equipped.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;
	
	/// <summary>
	/// The default ability sets that will be granted to the hero when this weapon is equipped.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroAbilitySet> DefaultWeaponAbilities;

	/// <summary>
	/// The special ability sets that will be granted to the hero when this weapon is equipped.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroSpecialAbilitySet> SpecialWeaponAbilities;

	/// <summary>
	/// The base damage of the weapon, which can be scaled based on the hero's stats
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	/// <summary>
	/// The soft reference to the weapon's icon texture, which can be used in the UI
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};
