// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarriorWidgetBase.generated.h"

class UHeroUIComponent;
class UEnemyUIComponent;

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UWarriorWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	/// Called when the widget is initialized
	virtual void NativeOnInitialized() override;

	/// Blueprint event triggered when the owning hero UI component is initialized
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Hero UI Component Initialized"))
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* OwningHeroUIComponent);
	
	/// Blueprint event triggered when the owning enemy UI component is initialized
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Enemy UI Component Initialized"))
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);

public:
	/// Initializes the widget for an enemy actor by retrieving its Enemy UI Component and calling the corresponding Blueprint event
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);
};
