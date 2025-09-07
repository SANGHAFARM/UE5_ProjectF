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
	// 풀링된 오브젝트를 활성화/비활성화
	virtual void PooledObjectSetActive_Implementation(bool IsActive) override;

	// 풀링된 오브젝트를 비활성화
	virtual void PooledObjectDeactivate_Implementation() override;

	// 풀링된 오브젝트의 인덱스 설정
	virtual void SetPoolIndex_Implementation(int32 Index) override;

	// 인덱스 Getter
	virtual int32 GetPoolIndex_Implementation() const override;

	// 현재 오브젝트의 활성화 여부 체크
	virtual bool IsPoolActive_Implementation() const override;

	// 풀링된 오브젝트의 생명 주기 설정
	virtual void SetPooledObjectLifeSpan_Implementation(float LifeTime) override;
	
protected:
	uint8 bIsActive : 1;
	float LifeSpan = 0.0f;
	int PoolIndex;

	FTimerHandle LifeSpanTimer;
};
