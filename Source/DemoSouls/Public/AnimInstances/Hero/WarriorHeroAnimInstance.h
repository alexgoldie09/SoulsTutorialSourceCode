// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "WarriorHeroAnimInstance.generated.h"
class AWarriorHeroCharacter;

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorHeroAnimInstance : public UWarriorCharacterAnimInstance
{
	GENERATED_BODY()

public:
	/// Used to initialise the animation properties
	virtual void NativeInitializeAnimation() override;
	/// Used to update the animation properties every frame
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	/// Reference to the owning warrior hero character
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	AWarriorHeroCharacter* OwningHeroCharacter;

	/// Reference to the character's relax state
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	/// Reference to the threshold for entering the relax state
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	float IdleElapsedTime;
};
