
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectUnrest/GAS/PUGameplayEffect.h"
#include "ProjectUnrest/Player/NadiaPlayerState.h"
#include "GameplayTagsManager.h"
#include "EffectPipelineComponent.generated.h"

/*
* This component is intended to keep track of all player upgrades intended to apply at specific times in the damage pipeline - such as multiplicative damage effects that 
*	should apply to base damage before further, additive damage increases, or conditional effects that should apply based on which type of battery the player just fired. 
* 
* Effects are stored when the player initially received the upgrade, and that gameplay effect and its associated pipeline gameplay tag should be defined in the upgrade's data asset.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTUNREST_API UEffectPipelineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEffectPipelineComponent() = default;

protected:
	virtual void BeginPlay() override;

	/* Applies all persistent GE's to Nadia that are associated with the passed PipelineTag. */
	UFUNCTION(BlueprintCallable)
	void ApplyPipelineEffects(FGameplayTag PipelineTag);

	/* Removes all persistent GE's from Nadia that are associated with the passed PipelineTag. */
	UFUNCTION(BlueprintCallable)
	void RemovePipelineEffects(FGameplayTag PipelineTag);

	/* Add a GameplayEffect to the PipelineComponent that will be associated with the passed PipelineTag. */
	UFUNCTION(BlueprintCallable)
	void StoreConditionalEffect(FGameplayTag PipelineTag, TSubclassOf<UPUGameplayEffect> ConditionalGE);

	/* Returns true/false whether the pipeline contains any GE's associated with the passed PipelineTag. */
	UFUNCTION(BlueprintCallable)
	bool PipelineContainsEffectsOfTag(FGameplayTag PipelineTag) const;

private:
	TMap<FGameplayTag, TArray<TSubclassOf<UPUGameplayEffect>>> PipelineMap {};

	UPUAbilitySystemComponent* PlayerASC = nullptr;

	FGameplayEffectContextHandle ContextHandle{};

	/* This seems to be intended for replication/server-side prediction? Need it to apply GE specs to self. */
	FPredictionKey PredictionKey;

	
};
