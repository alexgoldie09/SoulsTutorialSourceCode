// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"

class AWarriorEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<AWarriorEnemyCharacter*>&, SpawnedEnemies);
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()

public:
	/// Waits for a gameplay event with the specified tag and spawns enemies when the event is received.
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (DisplayName = "Wait Gameplay Event And Spawn Enemies", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
		int32 NumToSpawn,
		const FVector& SpawnOrigin,
		float RandomSpawnRadius);

	/// Called when the task is finished spawning enemies.
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	/// Called when the task fails to spawn enemies.
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;

protected:
	//~ Begin UAbilityTask interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~ End UAbilityTask interface

private:
	/// Cached gameplay tag to listen for.
	FGameplayTag CachedEventTag;
	/// Cached soft class pointer to the enemy character to spawn.
	TSoftClassPtr<AWarriorEnemyCharacter> CachedSoftEnemyClassToSpawn;
	/// Cached number of enemies to spawn.
	int32 CachedNumToSpawn;
	/// Cached spawn origin.
	FVector CachedSpawnOrigin;
	/// Cached random spawn radius.
	float CachedRandomSpawnRadius;
	/// Cached delegate handle for the gameplay event.
	FDelegateHandle DelegateHandle;

	/// Called when the gameplay event is received. Spawns enemies and broadcasts the OnSpawnFinished delegate.
	void OnGameplayEventReceived(const FGameplayEventData* InPayLoad);

	/// Called when the enemy class is loaded. Spawns enemies and broadcasts the OnSpawnFinished delegate.
	void OnEnemyClassLoaded();
};
