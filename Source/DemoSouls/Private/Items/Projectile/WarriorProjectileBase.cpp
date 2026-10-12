// Alex Goldberg rights reserved


#include "Items/Projectile/WarriorProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"

// Sets default values
AWarriorProjectileBase::AWarriorProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize the projectile collision box and set it as the root component
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);

	// Set up collision settings for the projectile collision box
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Set collision to query only, no physics simulation
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // Set collision response to block pawns (characters)
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Set collision response to block world static objects (e.g., walls, floors)
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block); // Set collision response to block world dynamic objects (e.g., movable objects)
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit); // Bind the OnProjectileHit function to the OnComponentHit event
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap); // Bind the OnProjectileBeginOverlap function to the OnComponentBeginOverlap event
	
	// Initialize the projectile Niagara component and attach it to the root component
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	// Initialize the projectile movement component and set its properties
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 700.f; // Set the initial speed of the projectile
	ProjectileMovementComponent->MaxSpeed = 900.f; // Set the maximum speed of the projectile
	ProjectileMovementComponent->Velocity = FVector(1.f, 0.f, 0.f); // Set the initial velocity of the projectile to zero
	ProjectileMovementComponent->ProjectileGravityScale = 0.f; // Set the gravity scale to zero (no gravity effect on the projectile)

	InitialLifeSpan = 4.f; // Set the initial lifespan of the projectile to 4 seconds

}

// Called when the game starts or when spawned
void AWarriorProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Set collision response to overlap pawns (characters)
	}
}

/// <summary>
/// Called when the projectile hits another actor.
/// It checks if the hit actor is a pawn and if it is hostile to the instigator of the projectile. If the hit pawn is blocking, it sends a successful block event to the hit pawn. Otherwise, it applies damage to the hit pawn. Finally, it destroys the projectile.
/// </summary>
/// <param name="HitComponent">The component that was hit.</param>
/// <param name="OtherActor">The actor that was hit.</param>
/// <param name="OtherComp">The component of the other actor that was hit.</param>
/// <param name="NormalImpulse">The normal impulse applied to the hit component.</param>
/// <param name="Hit">The hit result containing information about the hit.</param>
void AWarriorProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Call the Blueprint event to spawn hit effects at the impact point
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);

	// Check if the other actor is a pawn and if it is hostile to the instigator of the projectile
	APawn* HitPawn = Cast<APawn>(OtherActor);

	if (!HitPawn || !UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
	{
		Destroy();
		return;
	}

	bool bIsValidBlock = false;

	// Check if the hit pawn is currently blocking
	if (const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitPawn, WarriorGameplayTags::Player_Status_Blocking))
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(this, HitPawn);

	// Prepare the gameplay event data to send to the hit pawn
	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = HitPawn;

	// If the hit pawn is blocking and the block is valid, send a successful block event to the hit pawn
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			EventData);
	}
	else
	{
		HandleApplyProjectileDamage(HitPawn, EventData);
	}

	Destroy();
}

/// <summary>
/// Called when the projectile begins overlapping with another actor.
/// It checks if the overlapping actor is a pawn and if it is hostile to the instigator of the projectile. If the overlapping pawn is hostile, it applies damage to the pawn.
/// </summary>
/// <param name="OverlappedComponent">The component that began overlapping.</param>
/// <param name="OtherActor">The actor that began overlapping.</param>
/// <param name="OtherComp">The component of the other actor that began overlapping.</param>
/// <param name="OtherBodyIndex">The body index of the other component.</param>
/// <param name="bFromSweep">Whether the overlap was caused by a sweep.</param>
/// <param name="SweepResult">The hit result containing information about the overlap.</param>
void AWarriorProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedActors.Contains(OtherActor))
		return;

	OverlappedActors.AddUnique(OtherActor);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		FGameplayEventData EventData;
		EventData.Instigator = GetInstigator();
		EventData.Target = HitPawn;
		
		if (UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
			HandleApplyProjectileDamage(HitPawn, EventData);
	}
}

void AWarriorProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& EventData)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("ProjectileDamageEffectSpecHandle is not valid. Make sure to set it before applying damage."));

	if (const bool bWasApplied = UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle))
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitPawn,
			WarriorGameplayTags::Shared_Event_HitReact,
			EventData);
	}
}

