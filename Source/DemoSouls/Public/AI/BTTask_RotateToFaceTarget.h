// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

	// Constructor
	UBTTask_RotateToFaceTarget();

	//~ Begin UBTTaskNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~ End UBTTaskNode Interface

	// Check if the pawn has reached the desired angle precision to face the target actor
	bool HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const;

	// The angle precision for the rotation task
	UPROPERTY(EditAnywhere, Category = "Face Target")
	float AnglePrecision;

	// The speed at which the character rotates to face the target
	UPROPERTY(EditAnywhere, Category = "Face Target")
	float RotationInterpSpeed;

	// The blackboard key selector for the target to face
	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector InTargetToFaceKey;
};
