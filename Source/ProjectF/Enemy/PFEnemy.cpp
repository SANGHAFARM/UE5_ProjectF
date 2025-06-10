// Fill out your copyright notice in the Description page of Project Settings.


#include "PFEnemy.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APFEnemy::APFEnemy()
{
	MaxHP = 20.0f;
}

void APFEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 현재 HP를 최대로 설정
	CurrentHP = MaxHP;
}

float APFEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}
	
	// 실제 적용된 데미지를 현재 체력에서 감소
	CurrentHP -= ActualDamage;

	// 체력을 0보다 작아지지 않도록 clamp
	CurrentHP = FMath::Clamp(CurrentHP, 0.0f, MaxHP);

	// 체력이 0 이하가 되면 사망 처리
	if (CurrentHP <= 0.0f)
	{
		// Hitmarker를 위한 값 처리
		bIsDead = true;
		// 사망 처리 함수 호출
		Die();
	}

	// 실제 대미지 반환
	return ActualDamage;
}

void APFEnemy::Die()
{
	// 이동 컴포넌트 비활성화하여 움직이지 않도록 설정
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);
	}

	// 캡슐 콜리전을 비활성화하여 래그돌이 캡슐 콜리전과 간섭하지 않도록 설정
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Mesh에 물리 활성화
	if (GetMesh())
	{
		// 모든 뼈대에 물리 시뮬레이션 활성화
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		//GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("EnemyRagdoll"));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	// AI 컨트롤러 빙의 해제
	if (GetController())
	{
		GetController()->UnPossess();
	}

	// 5초 후에 파괴
	SetLifeSpan(5.0f);
}
