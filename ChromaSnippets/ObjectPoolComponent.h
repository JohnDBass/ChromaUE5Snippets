#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ObjectPoolComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, IsBlueprintBase="true"))
class PROJECTUNREST_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolComponent() = default;

	/**
	 * Return a reference to the next available, inactive Actor in the pool. 
	 * If all Actors in the pool are active in the scene, a new Actor will be constructed and added to the pool. 
	*/
	UFUNCTION(BlueprintCallable)
	const AActor* GetAvailablePoolActor();

	/**
	 * Marks an Actor as inactive in the scene, and available to be re-used.
	 * Any other logic such as disabling of rendering/collision/physics etc should be done before calling this method.
	*/
	UFUNCTION(BlueprintCallable)
	void ReturnActorToPool(const AActor* ActorPtr);
									
	/*
	 * Whether you want to spawn a certain number of Actors on BeginPlay().
	 * Note: Actors spawn at the origin (0,0,0) of the current level by default unless overridden with SetNewPoolActorLocation/Rotation methods.
	 * New actor spawn properties: SetActorHiddenInGame(true), SetActorEnableCollision(false), MoveComponent gravity scale = 0.0f 
	*/
	UFUNCTION(BlueprintCallable)
	void SpawnInitialPoolActors(int32 NumOfActorsToInit);


	UFUNCTION(BlueprintCallable)
	void SetPooledActorClass(TSubclassOf<AActor> Class);

	UFUNCTION(BlueprintCallable)
	void SetNewPoolActorLocation(FVector NewActorLoc);

	UFUNCTION(BlueprintCallable)
	void SetNewPoolActorRotation(FRotator NewActorRot);	


private:
	/* The type of actors this pool will store, visible as a debug tool in editor. */
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TSet<const AActor*>ActiveSceneActors;

	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TSet<const AActor*>InactiveSceneActors;

	/* The index of the last actor returned from the object pool via GetAvailablePoolActor(). */
	int32 ReturnedActorIndex = 0;

	/* Number of active actors in the scene returned from this pool. */
	int32 NumActiveActors = 0;

	/* The default world location for Inactive actors spawned by the object pool. */
	FVector NewActorLocation = FVector(0.0f, 0.0f, 0.0f);

	/* The default world rotation for Inactive actors spawned by the object pool. */
	FRotator NewActorRotation = FRotator(0.0f, 0.0f, 0.0f);


	/* Member method that spawns disabled actors into the scene, adding them to the pool */
	void AddNewActorToPool();
};
