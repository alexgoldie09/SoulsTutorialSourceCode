// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "WarriorHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UHeroCombatComponent;
class UHeroUIComponent;

/**
 * 
 */
UCLASS()
class DEMOSOULS_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()
public:
	/// Sets default values for this character's properties
	AWarriorHeroCharacter();

	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface

	//~ Begin IPawnUIInterface Interface
	virtual UHeroUIComponent* GetHeroUIComponent() const override;
	//~ End IPawnUIInterface Interface

	/// <summary>
	/// Retrieves the UHeroCombatComponent instance associated with this hero character.
	/// </summary>
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.
	
	/// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
#pragma region Components
	/// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/// Hero combat component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;

	/// Hero UI component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UHeroUIComponent* HeroUIComponent;

#pragma endregion 
	
#pragma region Inputs
	/// Input configuration data asset containing input actions and mappings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	/// Callback function for handling movement input
	void Input_Move(const FInputActionValue& InputActionValue);
	
	/// Callback function for handling look input
	void Input_Look(const FInputActionValue& InputActionValue);
	
	/// Callback function for handling switch target input when triggered
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);

	/// Callback function for handling switch target input when completed
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);

	/// Cached switch direction for target switching
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	/// Callback function for handling ability input when pressed
	void Input_AbilityInputPresed(FGameplayTag InInputTag);
	/// Callback function for handling ability input when released
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
#pragma endregion
};
