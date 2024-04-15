
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CalculateImpactCrit.generated.h"

/**
 * This gameplay execution calculation should be called in the player's damage pipeline just after calculating the current battery's initial impact damage.
 * This GEEC will then calculate the base impact crit damage using the player's base crit multiplier attribute, saving the new impact damage attribute value
 *	before further damage upgrade effects are applied.
 */
UCLASS()
class PROJECTUNREST_API UCalculateImpactCrit : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UCalculateImpactCrit();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
