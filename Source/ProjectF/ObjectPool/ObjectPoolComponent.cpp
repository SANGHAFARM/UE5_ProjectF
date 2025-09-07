// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"

#include "Interface/ObjectPoolInterface.h"

// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	
}

AActor* UObjectPoolComponent::SpawnPooledObject()
{
	for (AActor* ArrayActor : PooledObjectsArray)
	{
		IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(ArrayActor);
		if (PoolableActor && PoolableActor->IsPoolActive_Implementation() == false)
		{
			ArrayActor->TeleportTo(FVector(0), FRotator(0));
			PoolableActor->SetPooledObjectLifeSpan_Implementation(PooledObjectLifespan);
			PoolableActor->PooledObjectSetActive_Implementation(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex_Implementation());

			return ArrayActor;
		}
	}

	if (SpawnedPoolIndexes.Num() > 0)
	{
		int PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledObjectIndex);
		
		if (PooledObjectsArray.IsValidIndex(PooledObjectIndex))
		{
			AActor* PoolActor = PooledObjectsArray[PooledObjectIndex];
			
			IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(PoolActor);
			if (PoolableActor)
			{
				PoolActor->TeleportTo(FVector(0), FRotator(0));
				
				PoolableActor->PooledObjectSetActive_Implementation(false);
				PoolableActor->SetPooledObjectLifeSpan_Implementation(PooledObjectLifespan);
				PoolableActor->PooledObjectSetActive_Implementation(true);
				SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex_Implementation());

				return PoolActor;
			}
		}
	}

	return nullptr;
}

void UObjectPoolComponent::OnPooledObjectDespawn(AActor* PoolActor)
{
	IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(PoolActor);
	if (PoolableActor)
	{
		SpawnedPoolIndexes.Remove(PoolableActor->GetPoolIndex_Implementation());
	}
	
	// SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}

int UObjectPoolComponent::GetPoolSize()
{
	if (PooledObjectsArray.Num() <= 0)
	{
		return 0;
	}

	return PoolSize;
}

AActor* UObjectPoolComponent::GetIndexedPooledObject(int Index)
{
	if (PooledObjectsArray.IsValidIndex(Index) == false)
	{
		return nullptr;
	}

	return PooledObjectsArray[Index];
}

// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectSubclass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			if (PooledObjectSubclass->ImplementsInterface(UObjectPoolInterface::StaticClass()) == false)
			{
				return;
			}

			PooledObjectsArray.Empty();
			for (int i = 0; i < PoolSize; i++)
			{
				AActor* PoolActor = World->SpawnActor<AActor>(PooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
				if (PoolActor)
				{
					IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(PoolActor);
					if (PoolableActor)
					{
						PoolableActor->PooledObjectSetActive_Implementation(false);
						PoolableActor->SetPoolIndex_Implementation(i);
						PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPoolComponent::OnPooledObjectDespawn);
						PooledObjectsArray.Add(PoolActor);
					}
					else
					{
						PoolActor->Destroy();
					}
					
					// PoolActor->SetActive(false);
					// PoolActor->SetPoolIndex(i);
					// PoolActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPoolComponent::OnPooledObjectDespawn);
					// PooledObjectsArray.Add(PoolActor);
				}
			}
		}
	}
}