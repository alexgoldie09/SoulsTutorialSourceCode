// Alex Goldberg rights reserved


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "WarriorDebugHelper.h"
#include "Components/UI/HeroUIComponent.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"

/// <summary>
/// Constructor for the UWarriorAttributeSet class.
/// Initializes default values for the attributes defined in this class.
/// </summary>
UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	InitDamageTaken(0.f);
}

/// <summary>
/// Called after a gameplay effect has been executed.
/// </summary>
void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Get the IPawnUIInterface from the target actor to update the UI
	if (!CachedPawnUIInterface.IsValid())
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());

	checkf(CachedPawnUIInterface.IsValid(), TEXT("Target actor does not implement IPawnUIInterface"));

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

	checkf(PawnUIComponent, TEXT("PawnUIComponent does not exist from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	
	// Check if the attribute modified is current health, and clamp its value between 0 and max health
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		
		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	// Check if the attribute modified is current rage, and clamp its value between 0 and max rage
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		
		SetCurrentRage(NewCurrentRage);

		if (GetCurrentRage() == GetMaxRage())
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(
				Data.Target.GetAvatarActor(),
				WarriorGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentRage() == 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(
				Data.Target.GetAvatarActor(),
				WarriorGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(
				Data.Target.GetAvatarActor(),
				WarriorGameplayTags::Player_Status_Rage_Full);
		
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(
				Data.Target.GetAvatarActor(),
				WarriorGameplayTags::Player_Status_Rage_None);
		}

		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
	}

	// Check if the attribute modified is damage taken, and update current health accordingly
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		// const FString DebugString = FString::Printf(
		// 	TEXT("Old health: %f, Damage Done: %f, New health: %f"),
		// 	OldHealth,
		// 	DamageDone,
		// 	NewCurrentHealth
		// );
		// Debug::Print(DebugString, FColor::Green);
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (GetCurrentHealth() <= 0.f)
		{
			// Handle character death logic here
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(
				Data.Target.GetAvatarActor(),
				WarriorGameplayTags::Shared_Status_Dead);
		}
	}
}
