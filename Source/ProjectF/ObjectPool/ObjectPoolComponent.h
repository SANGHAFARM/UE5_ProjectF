// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTF_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

	// 풀링했던 오브젝트를 월드에서 활성화 및 관리
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* SpawnPooledObject();

	// 오브젝트의 인덱스를 인데스 배열에서 제거
	UFUNCTION()
	void OnPooledObjectDespawn(AActor* PoolActor);

	// 풀링된 오브젝트들의 크기 return
	int GetPoolSize();
	// Index에 해당하는 풀링된 오브젝트(AActor) return
	AActor* GetIndexedPooledObject(int Index);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<AActor> PooledObjectSubclass;
	
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int PoolSize = 20;
	
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float PooledObjectLifespan = 0.0f;

	TArray<AActor*> PooledObjectsArray;
	TArray<int> SpawnedPoolIndexes;
};
