// Alex Goldberg rights reserved


#include "Components/UI/EnemyUIComponent.h"

#include "Widgets/WarriorWidgetBase.h"

/// <summary>
/// Registers a widget to be drawn for the enemy UI component.
/// </summary>	
/// @param InWidgetToRegister The widget to register for drawing.
void UEnemyUIComponent::RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

/// <summary>
/// Removes all registered enemy drawn widgets from the parent and clears the list.
/// </summary>
void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
		return;

	for (UWarriorWidgetBase* Widget : EnemyDrawnWidgets)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
		}
	}
	EnemyDrawnWidgets.Empty();
}
