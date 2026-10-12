// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentageChangedDelegate, float, NewPercent);
/**
 * 
 */
UCLASS()
class DEMOSOULS_API UPawnUIComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnPercentageChangedDelegate OnCurrentHealthChanged;
};
