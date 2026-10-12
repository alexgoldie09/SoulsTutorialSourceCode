// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FWarriorInputActionConfig
{
	GENERATED_BODY()

public:
	/// The GameplayTag associated with this input action. This tag is used to identify the input action in the input system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	/// The InputAction associated with this input action. This is the actual input action that will be triggered when the input is received.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

	/// Validates whether the input action configuration is valid.
	/// An input action configuration is considered valid if both the InputTag and InputAction are valid.
	bool IsValid() const
	{
		return InputTag.IsValid() && InputAction;
	}
};

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/// The default input mapping context for this character. This context defines the default set of input actions and their mappings.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	/// A list of native input actions for this character. Each entry associates a GameplayTag with an InputAction.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> NativeInputActions;

	/// A list of ability input actions for this character. Each entry associates a GameplayTag with an InputAction.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> AbilityInputActions;

	/// Finds a native input action by its associated GameplayTag.
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
	
};
