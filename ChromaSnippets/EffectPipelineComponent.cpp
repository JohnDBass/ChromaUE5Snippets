
#include "ProjectUnrest/Components/EffectPipelineComponent.h"


void UEffectPipelineComponent::BeginPlay()
{
	Super::BeginPlay();
	const ANadiaPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<ANadiaPlayerState>();

	if (ensure(PlayerState))
	{
		PlayerASC = Cast<UPUAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
	}
	if (ensureMsgf(PlayerASC, TEXT("Failed to cast/find Nadia's ASC")))
	{
		return;
	}
	PredictionKey = FPredictionKey::CreateNewPredictionKey(PlayerASC);
}


void UEffectPipelineComponent::ApplyPipelineEffects(FGameplayTag PipelineTag)
{
	TArray<TSubclassOf<UPUGameplayEffect>>* FoundMatchingArray = PipelineMap.Find(PipelineTag);
	if (ensureMsgf(FoundMatchingArray, TEXT("PipelineMap does not contain a key matching passed PipelineTag, PipelineGEs will not be applied.")))
	{
		return;
	}
	for (TSubclassOf<UPUGameplayEffect> ConditionalGE : *FoundMatchingArray)
	{
		FGameplayEffectSpecHandle handle = PlayerASC->MakeOutgoingSpec(ConditionalGE, 1, ContextHandle);
		handle.Data->AddDynamicAssetTag(PipelineTag);
	}	
}

void UEffectPipelineComponent::RemovePipelineEffects(FGameplayTag PipelineTag)
{
	TArray<TSubclassOf<UPUGameplayEffect>>* FoundMatchingArray = PipelineMap.Find(PipelineTag);

	if (FoundMatchingArray)
	{
		FGameplayTagContainer TempContainer;
		TempContainer.AddTag(PipelineTag);
		PlayerASC->RemoveActiveEffectsWithTags(TempContainer);
	}	
}




void UEffectPipelineComponent::StoreConditionalEffect(FGameplayTag PipelineTag, TSubclassOf<UPUGameplayEffect> ConditionalGE)
{
	FGameplayTag tag = PipelineTag.RequestDirectParent();
	
	if (!ensureAlwaysMsgf((tag.ToString() == FString("Pipeline")), TEXT("Tag passed to StoreConditionalEffect must be a Pipeline tag")))
		return;

	TArray<TSubclassOf<UPUGameplayEffect>>* FoundMatchingArray = PipelineMap.Find(PipelineTag);
	if (FoundMatchingArray)
	{
		FoundMatchingArray->Add(ConditionalGE);
	}
	else
	{
		FoundMatchingArray = new TArray<TSubclassOf<UPUGameplayEffect>>;
		FoundMatchingArray->Add(ConditionalGE);
		PipelineMap.Add(PipelineTag, *FoundMatchingArray);
	}
}

bool UEffectPipelineComponent::PipelineContainsEffectsOfTag(FGameplayTag PipelineTag) const
{
	if (PipelineMap.Find(PipelineTag) != nullptr)
		return true;

	return false;
}

