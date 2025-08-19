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

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* SpawnPooledObject();

	UFUNCTION()
	void OnPooledObjectDespawn(AActor* PoolActor);

	int GetPoolSize();
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
