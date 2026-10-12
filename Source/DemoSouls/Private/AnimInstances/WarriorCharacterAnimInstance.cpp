// Alex Goldberg rights reserved


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

/// Initialises the animation properties
void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	// Get pawn owner and cast to warrior character
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		// Initialise animation properties here
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

/// Updates the animation properties every frame
void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
		return;

	// Set ground speed by owner velocity
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	// Set acceleration by owner acceleration
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	// Set locomotion direction by owner velocity and rotation
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
