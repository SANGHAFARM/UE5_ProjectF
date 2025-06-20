// Fill out your copyright notice in the Description page of Project Settings.


#include "PFGameMode.h"

#include "Character/PFCharacterPlayer.h"
#include "Enemy/PFEnemySpawner.h"
#include "Kismet/GameplayStatics.h"

void APFGameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainTime = DefaultTime;

	GetWorldTimerManager().SetTimer(SurvivalTimerCountDownHandle, this, &APFGameMode::SurvivalTimerCountDown, 1.0f, true);
	
	// 스폰 액터 생성
	if (SpawnerClass)
	{
		SpawnerInstance = GetWorld()->SpawnActor<APFEnemySpawner>(SpawnerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APFGameMode::SpawnEnemyRandomTime, 2.0f, false);
}

void APFGameMode::SpawnEnemy()
{
	if (SpawnerInstance == nullptr)
	{
		return;
	}

	APFCharacterPlayer* Player = Cast<APFCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player)
	{
		if (Player->GetIsDead() == false)
		{
			SpawnerInstance->TrySpawnEnemy(Player);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		}
	}
}

void APFGameMode::SpawnEnemyRandomTime()
{
	float RandomTime = FMath::RandRange(2.0f, 4.0f);

	SpawnEnemy();
	
	// 타이머로 주기적 스폰
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APFGameMode::SpawnEnemyRandomTime, RandomTime, false);
}

void APFGameMode::SurvivalTimerCountDown()
{
	RemainTime -= 1;
	int32 NewTime = FMath::Clamp(RemainTime, 0, DefaultTime);

	// RemainTime을 감소시킨 후, 델리게이트에 바인딩된 HUD 함수를 호출하여 남은 시간 텍스트 업데이트
	OnUpdateSurvivalRemainTime.ExecuteIfBound(NewTime);
	
	if (NewTime <= 0)
	{
		GameOver();
	}
}

void APFGameMode::GameOver()
{
	GetWorldTimerManager().ClearTimer(SurvivalTimerCountDownHandle);

	if (RemainTime <= 0)
	{
		CallGameOverMenu(true);
	}
	else
	{
		CallGameOverMenu(false);
	}
}
