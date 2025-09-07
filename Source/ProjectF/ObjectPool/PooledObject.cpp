// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/PooledObject.h"

// Sets default values
APooledObject::APooledObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APooledObject::PooledObjectSetActive_Implementation(bool IsActive)
{
	IObjectPoolInterface::PooledObjectSetActive_Implementation(IsActive);

	bIsActive = IsActive;
	SetActorHiddenInGame(!IsActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObject::PooledObjectDeactivate_Implementation, LifeSpan, false);
}

void APooledObject::PooledObjectDeactivate_Implementation()
{
	IObjectPoolInterface::PooledObjectDeactivate_Implementation();

	PooledObjectSetActive_Implementation(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledObjectDespawn.Broadcast(this);
}

void APooledObject::SetPoolIndex_Implementation(int32 Index)
{
	IObjectPoolInterface::SetPoolIndex_Implementation(Index);

	PoolIndex = Index;
}

int32 APooledObject::GetPoolIndex_Implementation() const
{
	return PoolIndex;
}

bool APooledObject::IsPoolActive_Implementation() const
{
	return bIsActive;
}

void APooledObject::SetPooledObjectLifeSpan_Implementation(float LifeTime)
{
	IObjectPoolInterface::SetPooledObjectLifeSpan_Implementation(LifeTime);

	LifeSpan = LifeTime;
}