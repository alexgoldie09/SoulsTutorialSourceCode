// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_DamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class DEMOSOULS_API UGEExecCalc_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Constructor that adds attack power of the source, and defense power of the target to capture
	/// </summary>
	UGEExecCalc_DamageTaken();

	/// <summary>
	/// Executes the damage calculation based on the captured attributes
	/// </summary>
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
