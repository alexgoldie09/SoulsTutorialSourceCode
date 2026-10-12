// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UAbilityTask_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Default constructor for the ability task.
	/// </summary>
	UAbilityTask_ExecuteTaskOnTick();
	
	/// <summary>
	/// Executes a task on every tick while the ability is active.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	// ~ Begin UGameplayTask interface
	virtual void TickTask(float DeltaTime) override;
	// ~ End UGameplayTask interface
	
	/// <summary>
	/// Delegate called every tick while the ability is active.
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};
