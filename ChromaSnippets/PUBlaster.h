#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectUnrest/GAS/PUGameplayAbility.h"
#include "PUBlaster.generated.h"


class UPUAbilitySystemComponent;
class ABackpack;


/**
 *	An energy-gun that fires out battery charges. It gives the owner ability system component the shoot gameplay ability. When the shoot 
 *	ability is activated, it gets the current battery in the backpack
*/
UCLASS()
class PROJECTUNREST_API APUBlaster : public AActor
{
	GENERATED_BODY()
	
public:	
	APUBlaster();
	virtual void Tick(float DeltaTime) override;

	/** Caches references and initializes the state of the blaster. Required for blaster to function */
	UFUNCTION(BlueprintCallable, Category = "Blaster")
	void Init(UPUAbilitySystemComponent* _OwnerASC, ABackpack* _Backpack);

	/** Gives and activates current battery discharge ability and sends discharge event to owner ASC to trigger discharge upgrades */
	UFUNCTION(BlueprintCallable, Category = "Blaster")
	void Discharge(FHitResult HitScanResult);

protected:
	/** The blaster's static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blaster")
	UStaticMeshComponent* BlasterMesh = nullptr;

	/** The ability system component of the actor that owns this blaster */
	UPROPERTY(BlueprintReadOnly, Category = "Blaster")
	UPUAbilitySystemComponent* OwnerASC = nullptr;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Blaster")
	const ABackpack* GetBackpack() const;

private:
	/** Abilities that should be granted to the owner for having a blaster */
	UPROPERTY(EditDefaultsOnly, Category = "Blaster", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UPUGameplayAbility>> DefaultAbilities;

	/** The tag that will go on the event made when the current battery is discharged */
	UPROPERTY(EditDefaultsOnly, Category = "Blaster", meta = (AllowPrivateAccess = "true"))
	FGameplayTag DischargeEventTag;

	/** The backpack on the owner of this blaster and which the blaster is dependent on */
	ABackpack* Backpack = nullptr;

	/** The material slot index for the emissive material that changes when current battery changes */
	int32 EmissiveMaterialSlot = 1;


	/** Gives the OwnerASC abilities that come with a blaster */
	void GiveDefaultAbilities();

	/** Sets the blaster's emissive material to that of the current battery */
	UFUNCTION()
	void UpdateEmissiveMaterial();
};
