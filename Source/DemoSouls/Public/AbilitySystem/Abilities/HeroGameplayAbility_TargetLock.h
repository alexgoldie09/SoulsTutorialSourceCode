// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UInputMappingContext;
class UWarriorWidgetBase;
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	// ~ Begin UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~ End UGameplayAbility interface

	/// <summary>
	/// Called every tick while the ability is active to update the target lock state.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);
	
private:
#pragma region Target Lock Ability Functions
	/// <summary>
	/// Attempts to lock on to targets by performing a box trace and collecting available actors.
	/// </summary>
	void TryLockOnTarget();
	/// <summary>
	/// Performs a box trace to find available actors to lock on to and adds them to the LockedOnTargets array.
	/// </summary>
	void GetAvailableActorsToLock();
	/// <summary>
	/// Gets the nearest target from the available actors based on distance to the character.
	/// </summary>
	/// <param name="InAvailableActors">The array of available actors to consider for locking on.</param>
	/// <returns>The nearest target actor, or nullptr if no valid targets are found.</returns>
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);
	/// <summary>
	/// Gets the available actors around the current locked on target and separates them into left and right arrays.
	/// </summary>
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
	/// <summary>
	/// Draws the target lock widget for the currently locked on target.
	/// </summary>
	void DrawTargetLockWidget();
	/// <summary>
	/// Sets the position of the target lock widget based on the current locked on target.
	/// </summary>
	void SetTargetLockWidgetPosition();
	/// <summary>
	/// Initializes the movement of the character to face the locked on target.
	/// </summary>
	void InitTargetLockMovement();
	/// <summary>
	/// Initializes the input mapping context for the target lock ability.
	/// </summary>
	void InitTargetLockMappingContext();
#pragma endregion
	
#pragma region Target Lock Ability Cleanup
	/// <summary>
	/// Cancels the target lock ability and clears the LockedOnTargets array.
	/// </summary>
	void CancelTargetLockAbility();
	/// <summary>
	/// Cleans up the target lock ability by clearing the LockedOnTargets array.
	/// </summary>
	void Cleanup();
	/// <summary>
	/// Resets the movement of the character to its default state after target locking.
	/// </summary>
	void ResetTargetLockMovement();
	/// <summary>
	/// Resets the target lock mapping context from the input subsystem.
	/// </summary>
	void ResetTargetLockMappingContext();
#pragma endregion

#pragma region Target Lock Ability Properties
	/// <summary>
	/// The distance to perform the box trace for target locking.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance = 5000.f;

	/// <summary>
	/// The size of the box trace for target locking.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	/// <summary>
	/// The collision channels to consider when performing the box trace for target locking.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	/// <summary>
	/// Whether to show a persistent debug shape for the box trace when target locking.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistentDebugShape = false;

	/// <summary>
	/// The class of the widget to use for displaying the target lock UI.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;

	/// <summary>
	/// The speed at which the character rotates to face the locked on target.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockRotationInterpSpeed = 5.f;

	/// <summary>
	/// The maximum walk speed of the character while target locking.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockMaxWalkSpeed = 150.f;

	/// <summary>
	/// The distance to offset the camera when target locking.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockCameraOffsetDistance = 20.f;

	/// <summary>
	/// The input mapping context for the target lock ability.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	UInputMappingContext* TargetLockMappingContext;
#pragma endregion

#pragma region TargetLock Ability Private Properties
	/// <summary>
	/// The array of actors that are currently locked on to by the target lock ability.
	/// </summary>
	UPROPERTY()
	TArray<AActor*> LockedOnTargets;

	/// <summary>
	/// The actor that is currently locked on to by the target lock ability.
	/// </summary>
	UPROPERTY()
	AActor* CurrentLockedOnTargetActor;

	/// <summary>
	/// The widget instance that is currently drawn for the target lock UI.
	/// </summary>
	UPROPERTY()
	UWarriorWidgetBase* DrawnTargetLockWidget;

	/// <summary>
	/// The size of the target lock widget.
	/// </summary>
	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	/// <summary>
	/// The cached default maximum walk speed of the character before target locking.
	/// </summary>
	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.f;
#pragma endregion
};
