
#include "ProjectUnrest/GAS/ExecutionCalcs/CalculateImpactCrit.h"
#include "ProjectUnrest/GAS/PUAbilitySystemComponent.h"
#include "ProjectUnrest/GAS/AttributeSets/CombatSets/CombatAttributeSet.h"
#include "ProjectUnrest/GAS/AttributeSets/CombatSets/NadiaCombatAttributeSet.h"


struct FCritDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentBatteryImpactDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(NadiaCriticalMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TotalDamage);

	FCritDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UNadiaCombatAttributeSet, CurrentBatteryImpactDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UNadiaCombatAttributeSet, NadiaCriticalMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, TotalDamage, Source, false);
	}
};

static const FCritDamageStatics& CritDamageStatics()
{
	static FCritDamageStatics DmgStatics;
	return DmgStatics;
}


UCalculateImpactCrit::UCalculateImpactCrit()
{
	RelevantAttributesToCapture.Add(CritDamageStatics().CurrentBatteryImpactDamageDef);
	RelevantAttributesToCapture.Add(CritDamageStatics().NadiaCriticalMultiplierDef);
	RelevantAttributesToCapture.Add(CritDamageStatics().TotalDamageDef);
}

void UCalculateImpactCrit::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Obtain refs to the target and source ASC
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	// Get the owning GameplayEffect spec 
	const FGameplayEffectSpec& OwningSpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = OwningSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = OwningSpec.CapturedTargetTags.GetAggregatedTags();

	// Aggregator evaluate params used during attribute capture
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float PrimaryDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritDamageStatics().CurrentBatteryImpactDamageDef, EvaluationParameters, PrimaryDamage);

	float NadiaCriticalMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritDamageStatics().NadiaCriticalMultiplierDef, EvaluationParameters, NadiaCriticalMultiplier);

	float TotalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritDamageStatics().TotalDamageDef, EvaluationParameters, TotalDamage);

	TotalDamage = PrimaryDamage * NadiaCriticalMultiplier;
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(CritDamageStatics().TotalDamageProperty, EGameplayModOp::Override, TotalDamage));
}



