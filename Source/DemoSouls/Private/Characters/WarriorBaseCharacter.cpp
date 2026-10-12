// Alex Goldberg rights reserved


#include "Characters/WarriorBaseCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "MotionWarpingComponent.h"

/// Sets default values
AWarriorBaseCharacter::AWarriorBaseCharacter()
{
 	// Disable tick function for this character
	PrimaryActorTick.bCanEverTick = false;
	// Disable ticking on start for this character
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Disable decals on the character's mesh
	GetMesh()->bReceivesDecals = false;
	
	// Create Ability System Component and Attribute Set
	WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

/// Get the Ability System Component for this character
UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWarriorAbilitySystemComponent();
}

/// Get the Pawn Combat Component for this character (returns nullptr as this character does not have a combat component)
UPawnCombatComponent* AWarriorBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

/// Get the Pawn UI Component for this character (returns nullptr as this character does not have a UI component)
UPawnUIComponent* AWarriorBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

/// Called when this Pawn is possessed.
void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WarriorAbilitySystemComponent)
	{
		// Initialize the Ability System Component with the new controller
		WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);

		// Ensure the CharacterStartUpData is not null
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("CharacterStartUpData is null for %s. Please assign a valid DataAsset_StartUpDataBase in the character's properties."), *GetName());
	}
}

