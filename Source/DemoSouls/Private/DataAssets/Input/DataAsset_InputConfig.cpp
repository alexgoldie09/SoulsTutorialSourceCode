// Alex Goldberg rights reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"

/// Finds a native input action by its associated GameplayTag.
UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FWarriorInputActionConfig& Config : NativeInputActions)
	{
		if (Config.InputTag == InInputTag && Config.InputAction)
		{
			return Config.InputAction;
		}
	}
	return nullptr;
}
