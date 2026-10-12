// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"

class UEnemyUIComponent;
class UHeroUIComponent;
class UPawnUIComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEMOSOULS_API IPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/// <summary>
	/// Gets the Pawn UI Component associated with this pawn.
	/// </summary>
	virtual UPawnUIComponent* GetPawnUIComponent() const = 0;

	/// <summary>
	/// Gets the Hero UI Component associated with this pawn.
	/// </summary>
	virtual UHeroUIComponent* GetHeroUIComponent() const;

	/// <summary>
	/// Gets the Enemy UI Component associated with this pawn.
	/// </summary>
	virtual UEnemyUIComponent* GetEnemyUIComponent() const;
};
