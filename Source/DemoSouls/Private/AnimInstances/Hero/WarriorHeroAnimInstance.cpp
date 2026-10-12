// Alex Goldberg rights reserved


#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
#include "Characters/WarriorHeroCharacter.h"

/// Initialises the animation properties
void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Set hero character
	if (OwningCharacter)
		OwningHeroCharacter = Cast<AWarriorHeroCharacter>(OwningCharacter);
}

/// Updates the animation properties every frame
void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	// Update idle elapsed time and relax state
	// During acceleration, no relax state should be entered, so reset the idle elapsed time and relax state
	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	// When idle, enter relax state only when idle elapsed time is above the threshold
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = IdleElapsedTime >= EnterRelaxStateThreshold;
	}
}
