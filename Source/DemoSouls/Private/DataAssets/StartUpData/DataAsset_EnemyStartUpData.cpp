// Alex Goldberg rights reserved


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"

/// <summary>
/// Give the abilities to the ability system component
/// </summary>
void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	// Check if the enemy combat abilities array is empty
	if (!EnemyCombatAbilities.IsEmpty())
	{
		// Loop through the enemy combat abilities and give each ability to the ability system component
		for (const TSubclassOf<UWarriorEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			// If the ability set is not valid, continue to the next ability set
			if (!AbilityClass) continue;

			// Create a new ability spec and bind it to the ability system component
			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			// Give the ability to the ability system component
			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
