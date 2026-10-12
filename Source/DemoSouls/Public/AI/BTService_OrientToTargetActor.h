// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()

	/// Sets default values for this service's properties
	UBTService_OrientToTargetActor();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
	//~ End UBTNode Interface

	/// Represents the target object actor
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	/// The speed at which the AI will rotate to face the target actor
	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed;
};
