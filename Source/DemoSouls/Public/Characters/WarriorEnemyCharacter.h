// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UWidgetComponent;
class UEnemyUIComponent;
class UEnemyCombatComponent;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class DEMOSOULS_API AWarriorEnemyCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Constructor for the AWarriorEnemyCharacter class.
	/// Sets up default properties for AI-controlled enemy characters, including automatic AI possession, disabling controller rotation, and configuring character movement settings.
	/// </summary>
	AWarriorEnemyCharacter();

	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	//~ End IPawnUIInterface Interface

	/// <summary>
	/// Retrieves the UEnemyCombatComponent instance associated with this enemy character.
	/// </summary>
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }

	/// <summary>
	/// Retrieves the UBoxComponent instance for the left hand collision box.
	/// </summary>
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	/// <summary>
	/// Retrieves the UBoxComponent instance for the right hand collision box.
	/// </summary>
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }
	
protected:
	/// <summary>
	/// Called when the game starts or when spawned. Initializes the enemy health widget.
	/// </summary>
	virtual void BeginPlay() override;
	
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

	/// <summary>
	/// Called when a property is changed in the editor. Used to update the enemy character's properties and components when changes are made in the Unreal Editor.
	/// </summary>
#if WITH_EDITOR
	//~ Begin UObject Interface.
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface.
#endif
	
	
	/// <summary>
	/// Enemy combat component for AI-controlled enemy characters.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	/// <summary>
	/// Collision box for the left hand of the enemy character, used for detecting hits during combat.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* LeftHandCollisionBox;

	/// <summary>
	/// Name of the bone associated with the left hand collision box, used for attaching the collision box to the character's skeletal mesh.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandCollisionBoxBoneName;

	/// <summary>
	/// Collision box for the right hand of the enemy character, used for detecting hits during combat.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* RightHandCollisionBox;

	/// <summary>
	/// Name of the bone associated with the right hand collision box, used for attaching the collision box to the character's skeletal mesh.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightHandCollisionBoxBoneName;

	/// <summary>
	/// Enemy UI component for AI-controlled enemy characters.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

	/// <summary>
	/// Widget component for displaying the enemy's health bar in the UI.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

	/// <summary>
	/// Callback function for handling the beginning of an overlap event with the enemy's body collision boxes (left and right hand).
	/// This function is triggered when another actor overlaps with the collision boxes, allowing the enemy character to respond to combat interactions, such as taking damage or triggering attack effects.
	/// </summary>
	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	/// <summary>
	/// Initializes the enemy startup data by asynchronously loading the character's startup data asset and granting abilities to the ability system component.
	/// </summary>
	void InitEnemyStartupData();
};
