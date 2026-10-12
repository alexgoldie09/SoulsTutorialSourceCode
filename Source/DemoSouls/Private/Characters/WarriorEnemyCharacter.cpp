// Alex Goldberg rights reserved


#include "Characters/WarriorEnemyCharacter.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/WidgetComponent.h"
#include "Widgets/WarriorWidgetBase.h"

#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"

/// <summary>
/// Constructor for the AWarriorEnemyCharacter class. S
/// ets up default properties for AI-controlled enemy characters, including automatic AI possession, disabling controller rotation, and configuring character movement settings.
/// </summary>
AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	// Set this character to automatically possess AI when placed in the world or spawned
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Disable controller rotation for AI characters
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Set up character movement for AI characters
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxAcceleration = 1000.f;

	// Create enemy combat component for AI characters
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	// Create enemy UI component for AI characters
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));

	// Create enemy health bar for AI characters
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

	// Create left and right hand collision boxes for AI characters
	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
}

/// <summary>
/// Returns the pawn's combat component.
/// </summary>
UPawnCombatComponent* AWarriorEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

/// <summary>
/// Returns the pawn's UI component.
/// </summary>
UPawnUIComponent* AWarriorEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

/// <summary>
/// Returns the pawn's enemy UI component.
/// </summary>
UEnemyUIComponent* AWarriorEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

/// <summary>
/// Called when the game starts or when spawned. Initializes the enemy health widget.
/// </summary>
void AWarriorEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UWarriorWidgetBase* EnemyHealthWidget = Cast<UWarriorWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
		EnemyHealthWidget->InitEnemyCreatedWidget(this);
}

/// <summary>
/// Called when this Pawn is possessed by a controller. Initializes enemy startup data.
/// </summary>
void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartupData();
}

#if WITH_EDITOR
void AWarriorEnemyCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxBoneName))
	{
		// Update the attachment of the left collision boxes to the specified bones
		if (LeftHandCollisionBox)
			LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoxBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxBoneName))
	{
		// Update the attachment of the right collision boxes to the specified bones
		if (RightHandCollisionBox)
			RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoxBoneName);
	}
}
#endif

/// <summary>
/// Called when a collision box on the enemy character begins overlapping with another actor.
/// </summary>
void AWarriorEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
		if (UWarriorFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
}

/// <summary>
/// Initializes the enemy startup data by asynchronously loading the character's startup data asset and granting abilities to the ability system component.
/// </summary>
void AWarriorEnemyCharacter::InitEnemyStartupData()
{
	// If the character startup data is null, return early
	if (CharacterStartUpData.IsNull())
		return;

	// Asynchronously load the character startup data asset and grant abilities to the ability system component when loaded
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]()
		{
			// If the character startup data is successfully loaded, give the abilities to the ability system component
			if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
			{
				LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
				// Debug::Print(FString::Printf(TEXT("Enemy startup data loaded and abilities granted to %s"), *GetName()));
			}
		})
		);
}
