// Alex Goldberg rights reserved


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "WarriorDebugHelper.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Engine/AssetManager.h"

/// Waits for a gameplay event with the specified tag and spawns enemies when the event is received.
UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility,
	FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn,
	const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);

	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

/// Called when the task is activated. Binds to the gameplay event delegate.
void UAbilityTask_WaitSpawnEnemies::Activate()
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

/// Called when the task is destroyed. Unbinds from the gameplay event delegate.
void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	Delegate.Remove(DelegateHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

/// Called when the gameplay event is received. Requests async load of the enemy class to spawn.
void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayLoad)
{
	/// If the soft enemy class to spawn is valid, request async load of the class and bind to the OnEnemyClassLoaded delegate.
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
		);
	}
	/// If the soft enemy class to spawn is invalid, broadcast the DidNotSpawn delegate and end the task.
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());

		EndTask();
	}
	
}

/// Called when the enemy class is loaded.
/// Spawns the specified number of enemies at random locations within the specified radius and broadcasts the OnSpawnFinished delegate.
void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* EnemyClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	/// If the enemy class or world is invalid, broadcast the DidNotSpawn delegate and end the task.
	if (!EnemyClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());

		EndTask();

		return;
	}

	TArray<AWarriorEnemyCharacter*> SpawnedEnemies;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	/// Spawn the specified number of enemies at random locations within the specified radius.
	for (int32 i = 0; i < CachedNumToSpawn; i++)
	{
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);

		RandomLocation += FVector(0, 0, 150.f); // Adjust Z to spawn above ground
		
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		if (AWarriorEnemyCharacter* SpawnedEnemy = World->SpawnActor<AWarriorEnemyCharacter>(EnemyClass, RandomLocation, SpawnFacingRotation, SpawnParams))
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	/// Broadcast the OnSpawnFinished delegate with the spawned enemies or the DidNotSpawn delegate if no enemies were spawned.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		else
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
	}

	EndTask();
}
