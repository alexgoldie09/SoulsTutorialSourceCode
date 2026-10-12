// Alex Goldberg rights reserved

#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorTypes/WarriorCountDownAction.h"

/// <summary>
/// Gets the Warrior Ability System Component from an Actor.
/// </summary>
/// <param name="InActor">The Actor to get the Warrior Ability System Component from.</param>
/// <returns>The Warrior Ability System Component if found, otherwise nullptr.</returns>
UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	check(InActor);
	
	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

/// <summary>
/// Adds a Gameplay Tag to an Actor if it doesn't already have it.
/// </summary>
/// <param name="InActor">The Actor to add the Gameplay Tag to.</param>
/// <param name="TagToAdd">The Gameplay Tag to add.</param>
void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
		ASC->AddLooseGameplayTag(TagToAdd);
}

/// <summary>
/// Removes a Gameplay Tag from an Actor if it has it.
/// </summary>
/// <param name="InActor">The Actor to remove the Gameplay Tag from.</param>
/// <param name="TagToAdd">The Gameplay Tag to remove.</param>
void UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToAdd))
		ASC->RemoveLooseGameplayTag(TagToAdd);
}

/// <summary>
/// Checks if an Actor has a specific Gameplay Tag.
/// </summary>
/// <param name="InActor">The Actor to check.</param>
/// <param name="TagToCheck">The Gameplay Tag to check for.</param>
/// <returns>True if the Actor has the Gameplay Tag, otherwise false.</returns>
bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

/// <summary>
/// Checks if an Actor has a specific Gameplay Tag and returns the result as an EWarriorConfirmType.
/// </summary>
/// <param name="InActor">The Actor to check.</param>
/// <param name="TagToCheck">The Gameplay Tag to check for.</param>
/// <param name="OutConfirmType">The result of the check as an EWarriorConfirmType.</param>
void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
	EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

/// <summary>
/// Gets the Pawn Combat Component from an Actor that implements the IPawnCombatInterface.
/// </summary>
/// <param name="InActor">The Actor to get the Pawn Combat Component from.</param>
/// <returns>The Pawn Combat Component if the Actor implements the interface, otherwise nullptr.</returns>
UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
		return PawnCombatInterface->GetPawnCombatComponent();
	
	return nullptr;
}

/// <summary>
/// Gets the Pawn Combat Component from an Actor that implements the IPawnCombatInterface and returns the validity of the component.
/// </summary>	
UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = PawnCombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;
	return PawnCombatComponent;
}

/// <summary>
/// Checks if the TargetPawn is hostile to the QueryPawn based on their team IDs.
/// </summary>
bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);
	
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

/// <summary>
/// Gets the value of a FScalableFloat at a specific level.
/// </summary>
float UWarriorFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

/// <summary>
/// Computes the hit reaction direction tag based on the positions of the attacker and victim.
/// </summary>
FGameplayTag UWarriorFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim,
	float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNorm = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNorm);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNorm);

	if (CrossResult.Z < 0.f)
		OutAngleDifference *= -1.f;

	if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
		return WarriorGameplayTags::Shared_Status_HitReact_Front;
	else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
		return WarriorGameplayTags::Shared_Status_HitReact_Left;
	else if (OutAngleDifference < -135.f || OutAngleDifference > 135.f)
		return WarriorGameplayTags::Shared_Status_HitReact_Back;
	else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
		return WarriorGameplayTags::Shared_Status_HitReact_Right;

	return WarriorGameplayTags::Shared_Status_HitReact_Front;
}

/// <summary>
/// Checks if the block is valid based on the forward vectors of the attacker and defender.
/// </summary>
bool UWarriorFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender, float Threshold)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	return DotResult < Threshold; 
}

/// <summary>
/// Applies a Gameplay Effect Spec Handle to a target actor using the instigator's Warrior Ability System Component.
/// </summary>
bool UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
	const FGameplayEffectSpecHandle& InGameplayEffectSpecHandle)
{
	UWarriorAbilitySystemComponent* SourceASC = NativeGetWarriorASCFromActor(InInstigator);
	UWarriorAbilitySystemComponent* TargetASC = NativeGetWarriorASCFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InGameplayEffectSpecHandle.Data, TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

/// <summary>
/// Starts or cancels a countdown action based on the specified input.
/// </summary>
void UWarriorFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,
	float& OutRemainingTime, EWarriorCountDownActionInput CountDownInput,
	UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;

	// Get the world from the context object
	if (GEngine)
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	// If the world is not valid, return early
	if (!World)
		return;
	
	// Get the latent action manager from the world	
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	// Find any existing countdown action for the given callback target and UUID
	FWarriorCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FWarriorCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	// If the input is to start the countdown and no existing action is found, add a new countdown action
	if (CountDownInput == EWarriorCountDownActionInput::Start)
		if (!FoundAction)
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
				new FWarriorCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo));

	// If the input is to cancel the countdown and an existing action is found, cancel the action
	if (CountDownInput == EWarriorCountDownActionInput::Cancel)
		if (FoundAction)
			FoundAction->CancelAction();
}

