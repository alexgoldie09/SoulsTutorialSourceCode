// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/// Binds a native input action to a callback function using the provided input configuration and input tag.
	/// @param InInputConfig The input configuration data asset containing the input actions.
	/// @param InInputTag The gameplay tag associated with the input action to bind.
	/// @param TriggerEvent The trigger event that determines when the callback function is invoked.
	/// @param ContextObject The object context for the callback function.
	/// @param Func The callback function to be invoked when the input action is triggered.
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, 
	const FGameplayTag& InInputTag,
	ETriggerEvent TriggerEvent, 
	UserObject* ContextObject, 
	CallbackFunc Func);

	/// Binds an ability input action to callback functions for pressed and released events using the provided input configuration.
	/// @param InInputConfig The input configuration data asset containing the input actions.
	/// @param ContextObject The object context for the callback functions.
	/// @param InputPressedFunc The callback function to be invoked when the input action is pressed.
	/// @param InputReleasedFunc The callback function to be invoked when the input action is released.
	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, 
	UserObject* ContextObject, 
	CallbackFunc InputPressedFunc,
	CallbackFunc InputReleasedFunc);
};

/// Implementation of the BindNativeInputAction function template.
template<class UserObject, typename CallbackFunc>
inline void UWarriorInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
const FGameplayTag& InInputTag,
ETriggerEvent TriggerEvent,
UserObject* ContextObject,
CallbackFunc Func)
{
	// Validate that the input configuration is not null before proceeding with the binding.
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding!"));

	// Attempt to find the native input action associated with the provided gameplay tag in the input configuration.
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
		// // Log a message indicating successful binding of the input action with the specified gameplay tag from the input configuration data asset.
		// UE_LOG(LogTemp, Log, TEXT("Successfully bound input action with tag '%s' from input config data asset '%s'."), *InInputTag.ToString(), *InInputConfig->GetName());
	}
	// else
	// {
	// 	// Log a warning if the input action associated with the provided gameplay tag is not found in the input configuration.
	// 	UE_LOG(LogTemp, Warning, TEXT("Input action with tag '%s' not found in input config data asset '%s'."), *InInputTag.ToString(), *InInputConfig->GetName());
	// }
}

/// Implementation of the BindAbilityInputAction function template.
template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig,
	UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	// Validate that the input configuration is not null before proceeding with the binding.
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding!"));

	// Attempt to find the ability input action associated with the provided gameplay tag in the input configuration.
	for (const FWarriorInputActionConfig& AbilityActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityActionConfig.IsValid()) continue;

		BindAction(AbilityActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityActionConfig.InputTag);
		BindAction(AbilityActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityActionConfig.InputTag);
	}
}
