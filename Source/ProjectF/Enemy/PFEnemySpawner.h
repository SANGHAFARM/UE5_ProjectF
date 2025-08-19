// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PFEnemySpawner.generated.h"

class APFEnemy;
class APFCharacterPlayer;
class UObjectPoolComponent;

UCLASS()
class PROJECTF_API APFEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APFEnemySpawner();

	// Player의 위치를 기준으로 Enemy 생성 함수
	void TrySpawnEnemy(const APFCharacterPlayer* PFCharacterPlayer);
	
protected:
	// Player의 위치를 기준으로 랜덤으로 Enemy를 생성할 위치를 리턴하는 함수
	FVector GetRandomSpawnLocation(const FVector& PlayerLocation, const float SightRadius, const float EnemySpawnRadius);

	void DecreaseSpawnedEnemies();

	UPROPERTY(EditAnywhere, Category = SpawnClass)
	TSubclassOf<APFEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TObjectPtr<UObjectPoolComponent> ObjectPoolComponent;

	UPROPERTY(VisibleAnywhere, Category = Enemy)
	int SpawnedEnemies = 0; 
};
