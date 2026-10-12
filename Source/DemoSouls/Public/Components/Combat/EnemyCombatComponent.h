// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	/// Called when the weapon hits a target actor.
	/// This function can be overridden in derived classes to implement custom behavior when a weapon hits a target.
	virtual void OnHitTargetActor(AActor* HitActor) override;

protected:
	/// Toggles the collision of the body collision boxes (left and right hand) based on the specified boolean value and EToggleDamageType.
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType) override;
};
