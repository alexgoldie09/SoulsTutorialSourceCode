// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "WarriorCharacterAnimInstance.generated.h"

class AWarriorBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()

public:
	/// Used to initialise the animation properties
	virtual void NativeInitializeAnimation() override;
	/// Used to update the animation properties every frame
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	/// Reference to the owning warrior character
	UPROPERTY()
	AWarriorBaseCharacter* OwningCharacter;
	/// Reference to the owning character's movement component
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
	/// Reference to the character's ground speed
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;
	/// Reference to the character's acceleration
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
	/// Reference to the character's movement direction
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
