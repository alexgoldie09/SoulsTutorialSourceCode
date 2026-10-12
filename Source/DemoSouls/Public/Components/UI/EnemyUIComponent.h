// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UWarriorWidgetBase;
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Registers a widget to be drawn for the enemy UI component.
	/// </summary>
	/// @param InWidgetToRegister The widget to register for drawing.
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister);

	/// <summary>
	/// Removes all registered enemy drawn widgets from the parent and clears the list.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetsIfAny();

private:
	/// <summary>
	/// A list of widgets that are registered to be drawn for the enemy UI component.
	/// </summary>
	TArray<UWarriorWidgetBase*> EnemyDrawnWidgets;
};
