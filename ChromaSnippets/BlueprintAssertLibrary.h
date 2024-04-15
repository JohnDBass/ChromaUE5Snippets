
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintAssertLibrary.generated.h"

/**
 * A static blueprint library intended to replicate CPP assert functionality. 
 */
UCLASS()
class PROJECTUNREST_API UBlueprintAssertLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 
	 * Assert that the passed Bool is true. 
	 * If Condition is false this node will be treated as a breakpoint, halting gameplay.
	 * @param BlueprintCaller: 
		A reference to the blueprint calling this Assert - use a reference to self.
	 * @param Condition:
		Passed boolean condition - ex: Is an actor reference null?
	*/
	UFUNCTION(BlueprintCallable, Category = "Debugging")
	static void AssertIsTrue(UObject* BlueprintCaller, const bool Condition);

	/**
	 * @brief
	 * Assert that the passed Bool is false. 
	 * If Condition is true this node will be treated as a breakpoint, halting gameplay.
	 * @param BlueprintCaller:
		A reference to the blueprint calling this Assert - use a reference to self.
	 * @param Condition:
		Passed boolean condition - ex: Is an actor reference null?
	*/
	UFUNCTION(BlueprintCallable, Category = "Debugging")
	static void AssertIsFalse(UObject* BlueprintCaller, const bool Condition);

	/**
	 * @brief
	 * Assert that the passed Object reference is null. 
	 * If the Object reference is not null this node will be treated as a breakpoint, halting gameplay.
	 * @param BlueprintCaller:
		A reference to the blueprint calling this Assert - use a reference to self.
	 * @param RefToCheck:
		The Object reference to check
	*/
	UFUNCTION(BlueprintCallable, Category = "Debugging")
	static void AssertIsNull(UObject* BlueprintCaller, UObject* RefToCheck);

	/**
	 * @brief
	 * Assert that the passed Object reference is not null. 
	 * If the Object reference is null this node will be treated as a breakpoint, halting gameplay.
	 * @param BlueprintCaller:
		A reference to the blueprint calling this Assert - use a reference to self.
	 * @param RefToCheck:
		The Object reference to check
	*/
	UFUNCTION(BlueprintCallable, Category = "Debugging")
	static void AssertIsNotNull(UObject* BlueprintCaller, UObject* RefToCheck);
};
