// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PFGameMode.generated.h"

DECLARE_DELEGATE_OneParam(FOnUpdateSurvivalRemainTimeDelegate, int32 /* NewRemainTime */);

class APFEnemySpawner;
/**
 * 
 */
UCLASS()
class PROJECTF_API APFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// HUD의 남은 시간을 업데이트하는 함수와 바인딩 할 델리게이트
	FOnUpdateSurvivalRemainTimeDelegate OnUpdateSurvivalRemainTime;

	// 게임 종료 조건 달성 시 호출되는 함수
	void GameOver();
	
	FORCEINLINE uint32 GetRemainTime() const { return RemainTime; }

protected:
	UPROPERTY(EditAnywhere, Category = Spawner)
	TSubclassOf<APFEnemySpawner> SpawnerClass;

	UPROPERTY()
	TObjectPtr<APFEnemySpawner> SpawnerInstance;

	FTimerHandle SpawnTimerHandle;

	// 적 Spawn 함수
	void SpawnEnemy();

	// 랜덤 시간마다 SpawnEnemy()를 호출하는 함수
	void SpawnEnemyRandomTime();

	// RemainTime을 1초씩 감소시키고 HUD에 업데이트
	void SurvivalTimerCountDown();

	// 블루프린트에서 게임 종료 메뉴를 띄우기 위한 함수
	UFUNCTION(BlueprintImplementableEvent)
	void CallGameOverMenu(const bool bIsClear);

private:
	UPROPERTY(EditAnywhere, Category = Time)
	uint32 DefaultTime = 60;
	
	UPROPERTY(EditAnywhere, Category = Time)
	uint32 RemainTime;

	FTimerHandle SurvivalTimerCountDownHandle;
};
