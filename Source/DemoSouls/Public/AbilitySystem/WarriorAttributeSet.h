// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AttributeSet.h"
#include "WarriorAttributeSet.generated.h"

/// <summary>
/// Macro to define attribute accessors for a given class and property name.
/// This macro generates getter, setter, and initializer functions for the specified attribute.
/// </summary>
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IPawnUIInterface;
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Constructor for the UWarriorAttributeSet class.
	/// Initializes default values for the attributes defined in this class.
	/// </summary>
	UWarriorAttributeSet();

	/// <summary>
	/// Called after a gameplay effect has been executed.
	/// </summary>
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/// <summary>
	/// Current health of the warrior.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentHealth)

	/// <summary>
	/// Maximum health of the warrior.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxHealth)

	/// <summary>
	/// Current rage of the warrior.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentRage)

	/// <summary>
	/// Maximum rage of the warrior.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxRage)

	/// <summary>
	/// Attack power of the warrior.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, AttackPower)

	/// <summary>
	/// Defense power of the warrior.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DefensePower)

	/// <summary>
	/// Damage taken by the warrior.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DamageTaken)

private:
	/// <summary>
	/// Cached pointer to the IPawnUIInterface for updating the UI.
	/// </summary>
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};
