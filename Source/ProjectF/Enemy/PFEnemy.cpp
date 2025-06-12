// Fill out your copyright notice in the Description page of Project Settings.


#include "PFEnemy.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/PFAIController.h"
#include "Components/SphereComponent.h"

APFEnemy::APFEnemy()
{
	AIControllerClass = APFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));

	LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
	
	// Enemy의 회전을 컨트롤러의 회전에 따르지 않도록 설정
	bUseControllerRotationYaw = false;

	// Enemy의 이동 방향에 따라 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	
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

void APFEnemy::EnableAttackCollision(FName InSectionName)
{
	if (InSectionName == TEXT("RightAttack"))
	{
		
	}
	else if (InSectionName == TEXT("LeftAttack"))
	{
		
	}
}

void APFEnemy::DisableAttackCollision(FName InSectionName)
{
	if (InSectionName == TEXT("RightAttack"))
	{
		
	}
	else if (InSectionName == TEXT("LeftAttack"))
	{
		
	}
}

void APFEnemy::OnAttackTaskEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

float APFEnemy::GetAIAttackRange()
{
	return 150.0f;
}

float APFEnemy::GetAITurnSpeed()
{
	return 10.0f;
}

void APFEnemy::SetAIAttackDelegate(const FAIAttackFinishedDelegate& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APFEnemy::AttackByAI()
{
	bIsAttacking = true;

	uint8 RandomSection = FMath::RandRange(0, 1);
	FName SectionName = (RandomSection == 0) ? TEXT("RightAttack") : TEXT("LeftAttack");

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}
