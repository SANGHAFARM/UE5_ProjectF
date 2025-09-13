// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawnDelegate, AActor*, PoolableActor); // UObject* 또는 AActor*로 변경

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTF_API IObjectPoolInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 오브젝트 비활성화 델리게이트
	FOnPooledObjectDespawnDelegate OnPooledObjectDespawn;
	
	// 풀에서 객체를 가져올 때 호출될 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pool")
	void PooledObjectSetActive(bool IsActive);

	// 풀에 객체를 반환할 때 호출될 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pool")
	void PooledObjectDeactivate();

	// 풀에서 객체에 인덱스를 설정하기 위한 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pool")
	void SetPoolIndex(int32 Index);

	// 풀에서 객체의 인덱스를 가져오기 위한 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pool")
	int32 GetPoolIndex() const;

	// 풀에서 객체의 활성화 상태를 가져오기 위한 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pool")
	bool IsPoolActive() const;

	// 풀에서 객체에 수명을 설정하기 위한 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pool")
	void SetPooledObjectLifeSpan(float LifeTime);
};
