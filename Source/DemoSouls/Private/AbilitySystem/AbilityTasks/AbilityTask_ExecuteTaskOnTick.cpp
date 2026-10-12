// Alex Goldberg rights reserved


#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"

/// <summary>
/// Default constructor for the ability task.
/// </summary>
UAbilityTask_ExecuteTaskOnTick::UAbilityTask_ExecuteTaskOnTick()
{
	bTickingTask = true;
}

/// <summary>
/// Executes a task on every tick while the ability is active.
/// </summary>
UAbilityTask_ExecuteTaskOnTick* UAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	UAbilityTask_ExecuteTaskOnTick* Node = NewAbilityTask<UAbilityTask_ExecuteTaskOnTick>(OwningAbility);

	return Node;
}

/// <summary>
/// Called every tick while the ability is active.
/// </summary>
void UAbilityTask_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates())
		OnAbilityTaskTick.Broadcast(DeltaTime);
	else
		EndTask();
}
