
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectUnrest/Actors/PUBattery.h"
#include "GameplayTagContainer.h"
#include "Backpack.generated.h"


class ACharacter;
class UPUGameplayAbility;
class UPUAbilitySystemComponent;
class APUBlaster;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBatteryChangedDelegate);


/*
*	An actor that has and manages batteries. Uses an index to track the current battery. When the current battery is
*	discharged, the backpack rechambers, rotating to the next battery. When the last battery is discharged, the backpack reloads, 
*	playing an animation, resetting the index back to zero, and recharging all the batteries.
*/
UCLASS()
class PROJECTUNREST_API ABackpack : public AActor
{
	GENERATED_BODY()
	

public:
	/** Event fired when the current battery changes, whether it is a new one or just recharged/discharged */
	UPROPERTY(BlueprintAssignable, Category = "Backpack")
	FBatteryChangedDelegate CurrentBatteryChangedEvent;

	/* Event fired when batteries are insterted or swapped */
	UPROPERTY(BlueprintAssignable, Category = "Backpack")
	FBatteryChangedDelegate BatteryOrderChangedEvent;
	
	ABackpack();

	/** Caches references, binds events, and initializes batteries. */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void Init(
		ACharacter* _OwningCharacter, 
		UPUAbilitySystemComponent* _ASC
	);

	/** 
	*	The calling function for rechamber functionality. Decides whether to reload or increment current battery index.
	*	Defers the visual changes to Rechamber_BP.
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void Rechamber_Exec();

	/**
	 *	The calling function for the reload functionality.
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void Reload_Exec();


	/** Creates a new battery actor of given type and replaces owned battery at given index */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void InsertNewBattery(TSubclassOf<APUBattery> NewBatteryClass, int32 ChamberIndex);

	/** Swaps owned batteries between given indices */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void SwapOwnedBatteries(int32 FirstBatteryIndex, int32 SecondBatteryIndex);

	/** Calls discharge on the current battery  */
	void DischargeCurrentBattery();


	#pragma region === Accessors ===

	/** Returns the next battery to be shot */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	const APUBattery* GetCurrentBattery() const;

	/** Returns the owned battery at the given index */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	const APUBattery* GetBatteryAtIndex(int32 Index) const;


	/** Returns the index of the next battery to be shot */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	int32 GetCurrentBatteryIndex() const;

	/** Returns the next battery, after current battery */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	int32 GetNextBatteryIndex() const;


	/** Returns the count of the current battery type */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	const int32 GetCurrentBatteryCount() const;

	#pragma endregion


protected:
	/** The blaster's static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backpack")
	UStaticMeshComponent* BackpackMesh = nullptr;

	/** The battery instances in the cylinder */
	UPROPERTY(BlueprintReadOnly, Category = "Backpack")
	TArray<APUBattery*> OwnedBatteries;

	/** Sets battery location to socket and attaches to it */
	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void AttachBatteryToSocket(APUBattery* Battery, int32 SocketIndex);

	/** Only Rechamber_Exec should call this function. Responsible for driving visual changes of rechambering. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void Rechamber_BP();

	/** Only Rechamber_Exec should call this function. Increments current battery index. */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void Rechamber_CPP();

	/** Only Reload_Exec should call this function. Responsible for driving visual changes of rechambering. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void Reload_BP();

	/** Only Reload_BP should call this function. Resets current battery index to zero and recharges all batteries. */
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void Reload_CPP();


private:
	/** Total number of batteries per cylinder (reload) */
	UPROPERTY(EditDefaultsOnly, Category = "Backpack", meta = (AllowPrivateAccess = "true"))
	int32 OwnedBatteriesCount = 6;

	/** The types of batteries the backpack starts with */
	UPROPERTY(EditDefaultsOnly, Category = "Backpack", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APUBattery>> InitialOwnedBatteryTypes;

	/** The gameplay ability that drives the backpack reload */
	UPROPERTY(EditDefaultsOnly, Category = "Backpack", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPUGameplayAbility> ReloadAbility;


	/** The character that owns (i.e., inits) this backpack */
	ACharacter* OwnerCharacter = nullptr;

	/** The ability system component on the owning character */
	UPUAbilitySystemComponent* OwnerASC = nullptr;

	/** The index of the next battery to be shot */
	int32 CurrentBatteryIndex = 0;

	/** The number of each type of battery in the cylinder */
	TMap<FGameplayTag, int32> BatteryTypeCounts;

	/** The material slot index for the emissive material of BackpackMesh*/
	int32 EmissiveMaterialSlot = 0;


	/** Creates initial battery instances from specified initial classes */
	void CreateInitialBatteries();

	/** Counts the number of each type of battery in the cylinder, storing the counts in a TMap */
	void CountBatteryTypes();

	/** Gives and activates the reload ability to the OwnerASC */
	void ActivateReloadAbility();

	/** Sets the backpack's emissive material to that of the current battery */
	UFUNCTION()
	void UpdateEmissiveMaterial();
};
