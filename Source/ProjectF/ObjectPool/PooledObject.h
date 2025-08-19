// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ObjectPoolInterface.h"
#include "PooledObject.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawnDelegate, APooledObject*, PoolActor);

UCLASS()
class PROJECTF_API APooledObject : public AActor, public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APooledObject();

	// ObjectPoolInterface
public:
	virtual void PooledObjectSetActive_Implementation(bool IsActive) override;
	virtual void PooledObjectDeactivate_Implementation() override;
	virtual void SetPoolIndex_Implementation(int32 Index) override;
	virtual int32 GetPoolIndex_Implementation() const override;
	virtual bool IsPoolActive_Implementation() const override;
	virtual void SetPooledObjectLifeSpan_Implementation(float LifeTime) override;

	// 오브젝트 비활성화
	// UFUNCTION(BlueprintCallable, Category = "Pooled Object")
	// void Deactivate();

	// 오브젝트 활성화/비활성화 설정
	// void SetActive(bool IsActive);
	
	// void SetPooledObjectLifeSpan(float LifeTime);
	// void SetPoolIndex(int Index);

	// bool IsActive();
	// int GetPoolIndex();
	
protected:
	uint8 bIsActive : 1;
	float LifeSpan = 0.0f;
	int PoolIndex;

	FTimerHandle LifeSpanTimer;
};
