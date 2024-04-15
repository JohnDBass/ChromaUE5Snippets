
#include "ProjectUnrest/Components/ObjectPoolComponent.h"
#include <ProjectUnrest/Character/Nadia.h>


void UObjectPoolComponent::SpawnInitialPoolActors(int32 NumInitActors)
{
	if (!ensure(ActorClass))
	{
		return;
	}

	ensureMsgf(NumInitActors > 0, TEXT("SpawnInitialPoolActors should be called with a value greater than 0. NumInitActors will default to 1"));
	if (NumInitActors <= 0)
	{
		NumInitActors = 1;
	}

	InactiveSceneActors.Reserve(NumInitActors);

	for (int32 i = 0; i < NumInitActors; ++i)
	{
		AddNewActorToPool();
	}
}

void UObjectPoolComponent::ReturnActorToPool(const AActor* ActorPtr)
{
	if (!ensureMsgf(ActorPtr != nullptr, TEXT("Attempting to return an invalid Actor ptr to the object pool!")))
	{
		return;
	}
	InactiveSceneActors.Add(ActorPtr);
	ActiveSceneActors.Remove(ActorPtr);
}

const AActor* UObjectPoolComponent::GetAvailablePoolActor()
{
	if (InactiveSceneActors.Num() == 0)
		AddNewActorToPool();

	const AActor* actor = *InactiveSceneActors.begin();
	ensureMsgf(actor != nullptr, TEXT("Retrieved null Actor ptr from pool!"));

	InactiveSceneActors.Remove(actor);
	ActiveSceneActors.Add(actor);
	return actor;
}

void UObjectPoolComponent::AddNewActorToPool()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, NewActorLocation, NewActorRotation, SpawnParameters);
	
	if (!ensureMsgf(NewActor != nullptr, TEXT("AddNewActorToPool() failed to create a valid NewActor!")))
	{
		return;
	}
	NewActor->SetActorHiddenInGame(true);
	NewActor->SetActorEnableCollision(false);

	UCharacterMovementComponent* MovementComponent = NewActor->GetComponentByClass<UCharacterMovementComponent>();
	if (MovementComponent)
	{
		MovementComponent->GravityScale = 0.0f;
	}

	InactiveSceneActors.Add(NewActor);
}



void UObjectPoolComponent::SetPooledActorClass(TSubclassOf<AActor> Class)
{
	if (!ensure(Class))
	{
		return;
	}

	ActorClass = Class;
}

void UObjectPoolComponent::SetNewPoolActorLocation(FVector NewActorLoc)
{
	NewActorLocation = NewActorLoc;
}

void UObjectPoolComponent::SetNewPoolActorRotation(FRotator NewActorRot)
{
	NewActorRotation = NewActorRot;
}

