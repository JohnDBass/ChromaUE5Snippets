
#include "ProjectUnrest/Game/SceneDirector.h"

ASceneDirector::ASceneDirector()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}


void ASceneDirector::BeginPlay()
{
	Super::BeginPlay();

	if (bIsEnemyDirector && !ensure(EnemySpawnManager))
	{
		this->Destroy();
		return;
	}

	for (FSpawnCard Cards : CardsToChooseFrom)
	{
		MaxRandomRange += Cards.CardWeight;
	}

	if (DirectorType == INSTANT && bIsEnemyDirector)
	{
		BuyEnemy();
		this->Destroy();
		return;
	}
	else if (DirectorType == BOTH && bIsEnemyDirector)
		BuyEnemy();
	

	const ANadiaPlayerState* PlrState = GetWorld()->GetFirstPlayerController()->GetPlayerState<ANadiaPlayerState>();
	const UAbilitySystemComponent* PlrASC = NULL;
	const UGameInfoAttributeSet* PlrGISet = NULL;

	if (ensure(PlrState))
	{
		PlrASC = PlrState->GetAbilitySystemComponent();
	}

	if (ensure(PlrASC))
	{
		PlrGISet = PlrASC->GetSetChecked<UGameInfoAttributeSet>();
		PlayerPower = PlrGISet->GetCurrentPlayerPower();
		MapsCompleted = PlrGISet->GetCurrentMapsCompleted();
		DifficultyModifier = PlrGISet->GetCurrentDirectorCreditDifficultyModifier();
	}
}

void ASceneDirector::AddCreditsPerTick()
{
	Credits += DifficultyModifier * (CredMultiplier * ( (1 + MapsCompletedCoeff * MapsCompleted) + (PlayerPowerCoeff * PlayerPower) ));
}

void ASceneDirector::BuyEnemy()
{
	if (!ensure(EnemySpawnManager))
	{
		this->Destroy();
		return;
	}

	WeightedCardChoice = FMath::RandRange(0, MaxRandomRange);

	for (FSpawnCard Card : CardsToChooseFrom)
	{
		if (Card.CardWeight >= WeightedCardChoice)
		{
			if (Credits >= Card.CreditCost)
			{
				Credits -= Card.CreditCost;
				EnemySpawnManager->SpawnEnemy_BP(Card.EnemyClass);
				BuyEnemy();
			}
			break;
		}
	}
}


void ASceneDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DirectorType == INSTANT)
		return;

	TimeSinceLastSpawn += DeltaTime;

	AddCreditsPerTick();

	if (TimeSinceLastSpawn >= PurchaseInterval && bIsEnemyDirector)
	{
		BuyEnemy();
		TimeSinceLastSpawn = 0.0f;
		bFailedToSpawn = false;
	}
}

const float ASceneDirector::GetCredits() const
{
	return Credits;
}

const float ASceneDirector::GetCredMultiplier() const
{
	return CredMultiplier;
}

void ASceneDirector::SetCredMultiplier(float Val)
{
	CredMultiplier = FMathf::Max(Val, 0.0f);
}