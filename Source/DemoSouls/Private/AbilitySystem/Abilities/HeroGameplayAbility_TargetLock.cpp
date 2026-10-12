// Alex Goldberg rights reserved


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"

#include "EnhancedInputSubsystems.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Controllers/WarriorHeroController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

/// <summary>
/// Activates the ability to lock on to targets.
/// It performs a box trace in front of the character to find potential targets and adds them to the LockedOnTargets array.
/// </summary>
/// <param name="Handle">The handle of the ability spec.</param>
/// <param name="ActorInfo">Information about the actor that owns the ability.</param>
/// <param name="ActivationInfo">Information about the activation of the ability.</param>
/// <param name="TriggerEventData">Optional event data that triggered the ability.</param>
void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

/// <summary>
/// Ends the ability to lock on to targets.
/// </summary>
/// <param name="Handle">The handle of the ability spec.</param>
/// <param name="ActorInfo">Information about the actor that owns the ability.</param>
/// <param name="ActivationInfo">Information about the activation of the ability.</param>
/// <param name="bReplicateEndAbility">Whether to replicate the end of the ability.</param>
/// <param name="bWasCancelled">Whether the ability was cancelled.</param>
void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	Cleanup();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/// <summary>
/// Called every tick while the ability is active to update the target lock state.
/// It checks if the current locked on target is valid, updates the position of the target lock widget,
/// and optionally overrides the character's rotation to face the locked on target.
/// </summary>
void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	// If the current locked on target actor is not valid or is dead, or if the hero character is dead, cancel the ability.
	if (!CurrentLockedOnTargetActor ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedOnTargetActor, WarriorGameplayTags::Shared_Status_Dead) ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Shared_Status_Dead))	
	{
		CancelTargetLockAbility();
		return;
	}

	// Update the position of the target lock widget to follow the current locked on target actor.
	SetTargetLockWidgetPosition();

	// Determine if the character's rotation should be overridden to face the locked on target.
	const bool bShouldOverrideRotation =
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling) &&
			!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking);

	// If the character's rotation should be overridden,
	// calculate the desired rotation to face the locked on target and interpolate towards it.
	if (bShouldOverrideRotation)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetHeroCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedOnTargetActor->GetActorLocation()
			);

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);

		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

/// <summary>
/// Switches the current locked on target to a new target based on the specified switch direction tag.
/// It retrieves the available actors to lock on, separates them into left and right arrays,
/// and selects the nearest target from the appropriate array based on the switch direction.
/// </summary>
void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	else
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);

	if(NewTargetToLock)
		CurrentLockedOnTargetActor = NewTargetToLock;
}

/// <summary>
/// Attempts to lock on to targets by performing a box trace and collecting available actors.
/// If no valid targets are found, the ability is cancelled.
/// </summary>
void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (LockedOnTargets.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedOnTargetActor = GetNearestTargetFromAvailableActors(LockedOnTargets);

	if (CurrentLockedOnTargetActor)
	{
		DrawTargetLockWidget();

		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

/// <summary>
/// Performs a box trace to find available actors to lock on to and adds them to the LockedOnTargets array.
/// </summary>
void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	// Clear the LockedOnTargets array before performing the box trace
	LockedOnTargets.Empty();
	
	// Array to store the results of the box trace
	TArray<FHitResult> BoxTraceHits; 

	// Perform a box trace in front of the character to find potential targets
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
		);

	// Iterate through the hit results and add unique actors to the LockedOnTargets array
	for (const FHitResult& Hit : BoxTraceHits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				LockedOnTargets.AddUnique(HitActor);
				// Debug::Print(HitActor->GetActorNameOrLabel());
			}
		}
	}
}

/// <summary>
/// Gets the nearest target from the available actors based on distance to the character.
/// </summary>
/// <param name="InAvailableActors">The array of available actors to consider for locking on.</param>
/// <returns>The nearest target actor, or nullptr if no valid targets are found.</returns>
AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(
	const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

/// <summary>
/// Gets the available actors around the current locked on target and separates them into left and right arrays.
/// </summary>
void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft,
	TArray<AActor*>& OutActorsOnRight)
{
	// If the current locked on target actor is not valid or if there are no locked on targets, cancel the ability.
	if (!CurrentLockedOnTargetActor || LockedOnTargets.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	// Get the location of the player character and calculate the normalized direction vectors to the current locked on target and available actors.
	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNorm = (CurrentLockedOnTargetActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	// Iterate through the locked on targets and determine if they are to the left or right of the current locked on target.
	for (AActor* AvailableActor : LockedOnTargets)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedOnTargetActor)
			continue;

		const FVector PlayerToAvailableNorm = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNorm, PlayerToAvailableNorm);

		if (CrossResult.Z > 0.f)
			OutActorsOnRight.AddUnique(AvailableActor);
		else
			OutActorsOnLeft.AddUnique(AvailableActor);
	}
}

/// <summary>
/// Draws the target lock widget for the currently locked on target.
/// </summary>
void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to set the TargetLockWidgetClass."));

		DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

		check(DrawnTargetLockWidget);

		DrawnTargetLockWidget->AddToViewport();
	}
}

/// <summary>
/// Sets the position of the target lock widget based on the current locked on target.
/// </summary>
void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	// If the target lock widget or the current locked on target actor is not valid, cancel the ability.
	if (!DrawnTargetLockWidget || !CurrentLockedOnTargetActor)
	{
		CancelTargetLockAbility();
		return;
	}

	FVector2D ScreenPosition;

	// Project the world location of the current locked on target actor to screen space.
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedOnTargetActor->GetActorLocation(),
		ScreenPosition,
		true
		);

	// If the target lock widget size is not set, find the size of the widget by iterating through its widget tree and looking for a USizeBox.
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			});
		}

	// Adjust the screen position to center the target lock widget on the locked on target actor.
	ScreenPosition -= (TargetLockWidgetSize / 2.f);

	// Set the position of the target lock widget in the viewport.
	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

/// <summary>
/// Initializes the movement of the character to face the locked on target.
/// It caches the default max walk speed and sets the character's max walk speed to the target lock max walk speed.
/// </summary>
void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

/// <summary>
/// Initializes the input mapping context for the target lock ability.
/// It retrieves the local player and adds the target lock mapping context to the enhanced input subsystem with a priority of 3.
/// </summary>
void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

/// <summary>
/// Cancels the target lock ability, ending it and cleaning up any necessary state.
/// </summary>
void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

/// <summary>
/// Cleans up the target lock ability by clearing the LockedOnTargets array and resetting the current locked on target actor.
/// </summary>
void UHeroGameplayAbility_TargetLock::Cleanup()
{
	LockedOnTargets.Empty();
	CurrentLockedOnTargetActor = nullptr;
	if (DrawnTargetLockWidget)
		DrawnTargetLockWidget->RemoveFromParent();

	DrawnTargetLockWidget = nullptr;

	TargetLockWidgetSize = FVector2D::ZeroVector;

	CachedDefaultMaxWalkSpeed = 0.f;
}

/// <summary>
/// Resets the movement of the character to its default state after target locking.
/// </summary>
void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
}

/// <summary>
/// Resets the input mapping context for the target lock ability by removing it from the enhanced input subsystem.
/// </summary>
void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo())
		return;
	
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}
