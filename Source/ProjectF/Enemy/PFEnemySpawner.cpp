// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/PFEnemySpawner.h"

#include "PFEnemy.h"
#include "Character/PFCharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APFEnemySpawner::APFEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APFEnemySpawner::TrySpawnEnemy(const APFCharacterPlayer* PFCharacterPlayer)
{
	if (PFCharacterPlayer == nullptr)
	{
		return;
	}

	if (EnemyClass == nullptr)
	{
		return;
	}

	FVector SpawnLocation = GetRandomSpawnLocation(PFCharacterPlayer->GetActorLocation(), PFCharacterPlayer->GetSightRadius(), PFCharacterPlayer->GetEnemySpawnRadius());

	if (SpawnLocation != FVector::ZeroVector)
	{
		// SpawnLocation이 ZeroVector가 아닐 때만 Enemy를 Spawn
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
	}
}

FVector APFEnemySpawner::GetRandomSpawnLocation(const FVector& PlayerLocation, const float SightRadius, const float EnemySpawnRadius)
{
	// 0~360도 중에서 임의 각도를 정한 후 방향 벡터 생성
	float Angle = FMath::FRandRange(0.f, 2 * PI);
	FVector2D RandDir2D = FVector2D(FMath::Cos(Angle), FMath::Sin(Angle));

	// SightRadius + 100 이상이면서 EnemySpawnRadius - 100 이하인 거리 
	float RandomDist = FMath::RandRange(SightRadius + 100.0f, EnemySpawnRadius - 100.0f);

	// RandDir2D의 FVector2D 값을 Z의 값이 0인 FVector로 변환 
	FVector Offset(RandDir2D.X, RandDir2D.Y, 0.f);
	
	// 플레이어 위치로부터 RandomDist 거리의 지점을 Location으로 저장
	FVector Location = PlayerLocation + (Offset * RandomDist);
	Location.Z = PlayerLocation.Z;

	// 바닥 위치 찾기
	FHitResult Hit;
	FVector Start = Location + FVector(0, 0, 1000);
	FVector End = Location - FVector(0, 0, 1000);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	if (bHit)
	{
		return Location;
	}

	return FVector::ZeroVector;
}


