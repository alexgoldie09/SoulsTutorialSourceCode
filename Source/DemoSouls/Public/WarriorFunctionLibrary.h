// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "WarriorTypes//WarriorEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

struct FScalableFloat;
class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Gets the Warrior Ability System Component from an Actor.
	/// </summary>
	/// <param name="InActor">The Actor to get the Warrior Ability System Component from.</param>
	/// <returns>The Warrior Ability System Component if found, otherwise nullptr.</returns>
	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	/// <summary>
	/// Adds a Gameplay Tag to an Actor if it doesn't already have it.
	/// </summary>
	/// <param name="InActor">The Actor to add the Gameplay Tag to.</param>
	/// <param name="TagToAdd">The Gameplay Tag to add.</param>
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	/// <summary>
	/// Removes a Gameplay Tag from an Actor if it has it.
	/// </summary>
	/// <param name="InActor">The Actor to remove the Gameplay Tag from.</param>
	/// <param name="TagToAdd">The Gameplay Tag to remove.</param>
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToAdd);

	/// <summary>
	/// Checks if an Actor has a specific Gameplay Tag.
	/// </summary>
	/// <param name="InActor">The Actor to check.</param>
	/// <param name="TagToCheck">The Gameplay Tag to check for.</param>
	/// <returns>True if the Actor has the Gameplay Tag, otherwise false.</returns>
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	/// <summary>
	/// Checks if an Actor has a specific Gameplay Tag and returns the result as an EWarriorConfirmType.
	/// </summary>
	/// <param name="InActor">The Actor to check.</param>
	/// <param name="TagToCheck">The Gameplay Tag to check for.</param>
	/// <param name="OutConfirmType">The result of the check as an EWarriorConfirmType.</param>
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);

	/// <summary>
	/// Gets the Pawn Combat Component from an Actor.
	/// </summary>
	/// <param name="InActor">The Actor to get the Pawn Combat Component from.</param>
	/// <returns>The Pawn Combat Component if found, otherwise nullptr.</returns>
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	/// <summary>
	/// Gets the Pawn Combat Component from an Actor and returns the result as an EWarriorValidType.
	/// </summary>
	/// <param name="InActor">The Actor to get the Pawn Combat Component from.</param>
	/// <param name="OutValidType">The result of the check as an EWarriorValidType.</param>
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	/// <summary>
	/// Checks if the TargetPawn is hostile to the QueryPawn.
	/// </summary>
	/// <param name="QueryPawn">The Pawn that is querying.</param>
	/// <param name="TargetPawn">The Pawn that is being queried.</param>
	/// <returns>True if the TargetPawn is hostile to the QueryPawn, otherwise false.</returns>
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	/// <summary>
	/// Gets the value of a Scalable Float at a specific level.
	/// </summary>
	/// <param name="InScalableFloat">The Scalable Float to evaluate.</param>
	/// <param name="InLevel">The level at which to evaluate the Scalable Float. Default is 1.</param>
	/// <returns>The value of the Scalable Float at the specified level.</returns>
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

	/// <summary>
	/// Computes the hit react direction tag based on the attacker and victim's positions.
	/// </summary>
	/// <param name="InAttacker">The Actor that is attacking.</param>
	/// <param name="InVictim">The Actor that is being attacked.</param>
	/// <param name="OutAngleDifference">The angle difference between the attacker and victim's forward vectors.</param>
	/// <returns>The computed hit react direction tag.</returns>
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);

	/// <summary>
	/// Checks if the block is valid based on the attacker and defender's positions.
	/// </summary>
	/// <param name="InAttacker">The Actor that is attacking.</param>
	/// <param name="InDefender">The Actor that is defending.</param>
	/// <param name="Threshold">The threshold value for determining a valid block.</param>
	/// <returns>True if the block is valid, otherwise false.</returns>
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender, float Threshold = 0.f);

	/// <summary>
	/// Applies a Gameplay Effect Spec Handle to a target actor.
	/// </summary>
	/// <param name="InInstigator">The Actor that is instigating the effect.</param>
	/// <param name="InTargetActor">The Actor that is the target of the effect.</param>
	/// <param name="InGameplayEffectSpecHandle">The Gameplay Effect Spec Handle to apply.</param>
	/// <returns>True if the effect was successfully applied, otherwise false.</returns>
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InGameplayEffectSpecHandle);

	/// <summary>
	/// Performs a countdown over a specified total time, updating at specified intervals.
	/// </summary>
	/// <param name="WorldContextObject">The world context object for the countdown.</param>
	/// <param name="TotalTime">The total time for the countdown.</param>
	/// <param name="UpdateInterval">The interval at which to update the remaining time.</param>
	/// <param name="OutRemainingTime">The remaining time in the countdown.</param>
	/// <param name="CountDownInput">The input action for the countdown (Start or Cancel).</param>
	/// <param name="CountDownOutput">The output action for the countdown (Updated, Completed, or Cancelled).</param>
	/// <param name="LatentInfo">The latent action info for the countdown.</param>
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject,
		float TotalTime,
		float UpdateInterval,
		float& OutRemainingTime,
		EWarriorCountDownActionInput CountDownInput,
		UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& CountDownOutput,
		FLatentActionInfo LatentInfo);
};
