// Alex Goldberg rights reserved


#include "AI/BTTask_RotateToFaceTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

/// Sets default values
UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Native Rotate To Face Target");
	AnglePrecision = 10.f;
	RotationInterpSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	InTargetToFaceKey.AddObjectFilter(
		this,
		GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey),
		AActor::StaticClass());
}

/// Initialize the task from the behavior tree asset
void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

/// Get the size of the instance memory for this task
uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory);
}

/// Get a static description of the task for debugging purposes
FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision %s is reached"), *KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the target actor from the blackboard
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetToFaceKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	// Get the owning pawn
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	// Get the instance memory for this task
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(Memory);

	// Store the owning pawn and target actor in the instance memory
	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;

	// Check if the memory is valid
	if (!Memory->IsValid())
	{
		// Reset the instance memory and return failure
		return EBTNodeResult::Failed;
	}

	// Check if the pawn has reached the desired angle precision to face the target actor
	if (HasReachedAnglePrecision(OwningPawn, TargetActor))
	{
		// Reset the instance memory and return success
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

/// Tick the task to smoothly rotate the pawn to face the target actor
void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Get the instance memory for this task
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);

	// Check if the memory is valid
	if (!Memory->IsValid())
	{
		// Finish the task with failure if the memory is invalid
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// Check if the pawn has reached the desired angle precision to face the target actor
	if (HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		// Reset the instance memory and finish the task with success
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		// Smoothly rotate the pawn to face the target actor
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
		Memory->OwningPawn->SetActorRotation(TargetRot);
	}
}

/// Check if the pawn has reached the desired angle precision to face the target actor
bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const
{
	// Calculate the angle between the pawn's forward vector and the direction to the target actor
	const FVector OwnerFoward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTargetNorm = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	// Calculate the dot product and angle difference
	const float DotResult = FVector::DotProduct(OwnerFoward, OwnerToTargetNorm);
	const float ClampedDotResult = FMath::Clamp(DotResult, -1.f, 1.f); // Guard against float rounding pushing this past Acos's valid domain
	const float AngleDiff = UKismetMathLibrary::DegAcos(ClampedDotResult);

	return AngleDiff <= AnglePrecision;
}
