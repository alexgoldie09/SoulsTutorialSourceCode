// Alex Goldberg rights reserved


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "WarriorDebugHelper.h"

/// Constructor for the Warrior AI Controller
/// Creates and sets up the AI perception component and sight configuration
AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// Create and setup values for AI sense configuration for sight
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f;
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	// Create and setup values for AI perception component
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

/// Returns the team attitude of this AI controller towards another actor
ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// Check if the other actor is a pawn
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	// Get the other actor's team agent interface and compare its team ID with this AI controller's team ID
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
		return ETeamAttitude::Hostile;

	return ETeamAttitude::Friendly;
	
}

/// Called when the game starts or when spawned
void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();

	// Check if the default PathFollowingComponent is set to CrowdFollowingComponent
	if (UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		// Set the crowd simulation state based on the bEnableDetourCrowdAvoidance property
		CrowdFollowingComponent->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		// Set the crowd avoidance quality based on the DetourCrowdAvoidanceQuality property
		switch (DetourCrowdAvoidanceQuality)
		{
			case 1: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
			case 2: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
			case 3: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
			case 4: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
			default: break;
		}

		// Set the avoidance group and groups to avoid for the crowd following component
		CrowdFollowingComponent->SetAvoidanceGroup(1);
		CrowdFollowingComponent->SetGroupsToAvoid(1);
		CrowdFollowingComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

/// Callback function for when the AI perception component updates its perception of an actor
void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// If the blackboard component is valid and the "TargetActor" key is not set,
	// and the stimulus was successfully sensed, set the "TargetActor" key to the perceived actor
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
			if(Stimulus.WasSuccessfullySensed() && Actor)
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
}
