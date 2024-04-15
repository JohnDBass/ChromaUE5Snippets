#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectUnrest/Enemies/EnemySpawnManager.h"
#include "Runtime/Core/Public/Misc/AssertionMacros.h"
#include "ProjectUnrest/Player/NadiaPlayerState.h"
#include "ProjectUnrest/GAS/AttributeSets/GameInfoSets/GameInfoAttributeSet.h"
#include "SceneDirector.generated.h"


/*
* The Scene Director manages the What and WHen of spawning - specify the types of Actor this director can spawn, along with their relative spawn weights (likelihood of being chosen for spawn).
* Directors may be instant - intended to spend their credits and destruct, continuous - intended to regularly gain and spend credits on the specified interval, or a mixture of both 
	- spending initial credits and then remaining in scene as a continuous director.
*/
USTRUCT(BlueprintType)
struct FSpawnCard
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "SpawnCardMembers")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "SpawnCardMembers")
	float CreditCost = 0.0f;

	/* How likely this card is chosen for purchase by the director. */
	UPROPERTY(EditAnywhere, Category = "SpawnCardMembers")
	int32 CardWeight = 0;
};

UENUM(BlueprintType)
enum DirectorType
{
	INSTANT UMETA(DisplayName = "Instant"),
	CONTINUOUS UMETA(DisplayName = "Continuous"),
	BOTH UMETA(DisplayName = "Both")
};

UCLASS()
class PROJECTUNREST_API ASceneDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	ASceneDirector();
	virtual void Tick(float DeltaTime) override;

	/* Returns this Director's current amount of Credits */
	UFUNCTION(BlueprintCallable, Category = "Spawn Director")
	const float GetCredits() const;

	/* Returns this Director's current CredMultiplier property */
	UFUNCTION(BlueprintCallable, Category = "Spawn Director")
	const float GetCredMultiplier() const;

	/* Sets this director's CredMultiplier property */
	UFUNCTION(BlueprintCallable, Category = "Spawn Director")
	void SetCredMultiplier(float val);

protected:
	virtual void BeginPlay() override;

private:

	/**
	 * @brief
	 * Chooses a random number between 0 and MaxRandomRange, then attempts to purchase as many of the chosen items as the director can afford.
	 * If the director cannot afford that item, purchasing attempts will end until the next PurchaseInterval.
	*/
	void BuyEnemy();
	void AddCreditsPerTick();

private:
	/* 
	*	What type of scene director is this?
	*	Instant: Instantly purchases items from the provided spawn list on BeginPlay, then destroys itself. Initial credits should be > 0.
	*	Continuous: Gains an amount of credits per second based on its credit formula. Will attempt to purchase items every PurchaseInterval until the level ends.
	*	Both: Attempts to instantly purchase items on BeginPlay, then acts as a Continous director.
	*/
	UPROPERTY(EditAnywhere, Category = "SceneDirector")
	TEnumAsByte<DirectorType> DirectorType = INSTANT;

	/* Will this director spawn enemies? In the future we can use directors to populate the map with items/lootboxes etc */
	UPROPERTY(EditAnywhere, Category = "SceneDirector")
	bool bIsEnemyDirector = true;

	/* Must point to an object of class EnemySpawnManager in the scene. Director destroys itself if this is not set. */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsEnemyDirector"), Category = "SceneDirector")
	AEnemySpawnManager* EnemySpawnManager = nullptr;

	/* 
	*	The objects this director will randomly choose between when it tries to buy something. 
	*	CardWeight determines how likely the object is to be chosen. 
	*/
	UPROPERTY(EditAnywhere, Category = "SceneDirector")
	TArray<FSpawnCard> CardsToChooseFrom;

	/*	
	*	Time interval (seconds) between enemy spawns. After this interval, the director will spawn enemies until it can no longer afford to spawn another.
	*	This timer only resets once the director is unable to afford more enemies, meaning the actual time between spawnings may fluctuate (by milliseconds).
	*/
	UPROPERTY(EditAnywhere, Category = "SceneDirector")
	float PurchaseInterval = 0.0f;

	/* Initial credits this director will spawn with. Should be greater than 0 on an instant director */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DirectorType == DirectorType::INSTANT || DirectorType == DirectorType::BOTH"), Category = "SceneDirector")
	float InitialCredits = 0.0f;

	UPROPERTY(VisibleInstanceOnly)
	float Credits = InitialCredits;

	/* Creds per second =  CredMultiplier x (1 + 0.5 x PlayTimeCoeff) + (PlayerPower * 0.1) */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DirectorType == DirectorType::CONTINUOUS || DirectorType == DirectorType::BOTH"), Category = "Credit Income Parameters")
	float CredMultiplier = 1.0f;

	/* Creds per second =  CredMultiplier x (1 + 0.5 x PlayTimeCoeff) + (PlayerPower * 0.1) */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DirectorType == DirectorType::CONTINUOUS || DirectorType == DirectorType::BOTH"), Category = "Credit Income Parameters")
	float MapsCompletedCoeff = 0.5f;

	/* Creds per second =  CredMultiplier x (1 + 0.5 x PlayTimeCoeff) + (PlayerPower * 0.1) */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DirectorType == DirectorType::CONTINUOUS || DirectorType == DirectorType::BOTH"), Category = "Credit Income Parameters")
	float PlayerPowerCoeff = 0.1f;


	UPROPERTY(VisibleAnywhere)
	float MapsCompleted = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float PlayerPower = 5.0f;

	UPROPERTY(VisibleAnywhere)
	float DifficultyModifier = 1.0f;

	float TimeSinceLastSpawn = 0.0f;

	/* Sum of the CardWeight of every spawn card assigned to this director. */
	int32 MaxRandomRange = 0;

	int32 WeightedCardChoice = 0;
	bool bFailedToSpawn = false;


	
};
