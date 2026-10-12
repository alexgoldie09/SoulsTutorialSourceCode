// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/UI/PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftWeaponMaterial);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBegin, FGameplayTag, AbilityInputTag, float, CooldownDuration, float, TimeRemaining);

UCLASS()
class DEMOSOULS_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Called when the current rage attribute changes.
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnPercentageChangedDelegate OnCurrentRageChanged;

	/// <summary>
	/// Called when the weapon is equipped.
	/// </summary>
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "UI")
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;

	/// <summary>
	/// Called when the ability icon slot is updated.
	/// </summary>
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "UI")
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;

	/// <summary>
	/// Called when the ability cooldown begins.
	/// </summary>
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "UI")
	FOnAbilityCooldownBegin OnAbilityCooldownBegin;
	
};
