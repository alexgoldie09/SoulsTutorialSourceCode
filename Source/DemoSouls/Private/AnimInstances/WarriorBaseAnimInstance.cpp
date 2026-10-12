// Alex Goldberg rights reserved


#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "GameplayTagContainer.h"
#include "WarriorFunctionLibrary.h"

/// Checks if the owner of this animation instance has a specific gameplay tag
bool UWarriorBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
		return UWarriorFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	
	return false;
}
