// Alex Goldberg rights reserved


#include "Characters/WarriorHeroCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WarriorInputComponent.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "WarriorDebugHelper.h"

// Sets default values
AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	/// Initialise capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	/// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/// Set up camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f); // Offset the camera above the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	/// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // The rate at which the character rotates to face the direction of movement
	GetCharacterMovement()->MaxWalkSpeed = 400.f; // Set max walk speed
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // Set braking deceleration

	/// Create the hero combat component
	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));

	/// Create the hero UI component
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

/// Get the pawn's combat component
UPawnCombatComponent* AWarriorHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

/// Get the pawn's UI component
UPawnUIComponent* AWarriorHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

/// Get the hero's UI component
UHeroUIComponent* AWarriorHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

/// Called when this Pawn is possessed.
void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// If the startup data is not null, load it synchronously and give the abilities to the ability system component
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
			LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
	}
}

/// Called to bind functionality to input
void AWarriorHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Check if the input data asset is null
	checkf(InputConfigDataAsset, TEXT("Input config data asset is null, can not proceed with binding!"));
	
	// Set up input subsystem
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem); // Debug for null values
	
	// Add the default mapping context to the input subsystem with a priority of 0
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	// Bind native input actions
	UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	// Bind ability input actions
	WarriorInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPresed, &ThisClass::Input_AbilityInputReleased);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);
}

/// Called when the game starts or when spawned
void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

}

/// Handles character movement based on input action value
void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// Get the movement vector from the input action value and the current control rotation
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	// Move the character forward/backward
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	// Move the character right/left
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

/// Handles character looking based on input action value
void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	// Get the look direction vector from the input action value
	const FVector2D LookDirection = InputActionValue.Get<FVector2D>();

	// Look right/left
	if (LookDirection.X != 0.f)
		AddControllerYawInput(LookDirection.X);

	// Look up/down
	if (LookDirection.Y != 0.f)
		AddControllerPitchInput(LookDirection.Y);
}

/// Handles switch target input when triggered by storing the switch direction for later use
void AWarriorHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

/// Handles switch target input when completed by sending a gameplay event to the ability system component based on the stored switch direction
void AWarriorHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	this, 
	SwitchDirection.X > 0.f ? WarriorGameplayTags::Player_Event_SwitchTarget_Right : WarriorGameplayTags::Player_Event_SwitchTarget_Left,
	Data
	);
}

/// Handles ability input when pressed by calling the corresponding function in the ability system component
void AWarriorHeroCharacter::Input_AbilityInputPresed(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

/// Handles ability input when released by calling the corresponding function in the ability system component
void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

