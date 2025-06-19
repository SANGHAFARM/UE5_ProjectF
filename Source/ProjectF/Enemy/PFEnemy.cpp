// Fill out your copyright notice in the Description page of Project Settings.


#include "PFEnemy.h"

#include "PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/PFAIController.h"
#include "Character/PFCharacterPlayer.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

APFEnemy::APFEnemy()
{
	AIControllerClass = APFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetCollisionProfileName(TEXT("EnemyMesh"));

	// HandCollision
	RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));
	RightHandCollision->SetRelativeLocation(FVector(-25.0f, 0.0f, 0.0f));
	RightHandCollision->SetSphereRadius(64.0f);
	RightHandCollision->SetCollisionProfileName(TEXT("NoCollision"));

	LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
	LeftHandCollision->SetRelativeLocation(FVector(25.0f, 0.0f, 0.0f));
	LeftHandCollision->SetSphereRadius(64.0f);
	LeftHandCollision->SetCollisionProfileName(TEXT("NoCollision"));

	// Icon
	EnemyIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EnemyIcon"));
	EnemyIcon->SetupAttachment(GetRootComponent());
	EnemyIcon->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	EnemyIcon->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	EnemyIcon->SetRelativeScale3D(FVector(0.15f));
	EnemyIcon->SetEnableGravity(false);
	EnemyIcon->bApplyImpulseOnDamage = false;
	EnemyIcon->bReplicatePhysicsToAutonomousProxy = false;
	EnemyIcon->SetGenerateOverlapEvents(false);
	EnemyIcon->bVisibleInReflectionCaptures = false;
	EnemyIcon->bVisibleInRealTimeSkyCaptures = false;
	EnemyIcon->bVisibleInRayTracing = false;
	EnemyIcon->SetVisibleInSceneCaptureOnly(true);

	// Audio
	LocomotionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("LocomotionSound"));
	LocomotionSound->SetupAttachment(GetRootComponent());
	LocomotionSound->bAutoActivate = false;
	
	// Enemy의 회전을 컨트롤러의 회전에 따르지 않도록 설정
	bUseControllerRotationYaw = false;

	// Enemy의 이동 방향에 따라 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APFEnemy::OnOverlapBegin);
	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APFEnemy::OnOverlapBegin);
	
	MaxHP = 100.0f;
	BaseDamage = 20.0f;
}

void APFEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 현재 HP를 최대로 설정
	CurrentHP = MaxHP;

	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if (LocomotionSound)
	{
		LocomotionSound->Activate(true);
	}
}

void APFEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TakeDamage 함수에서 bIsInterpolatingSpeedRecovery 값을 true로 설정
	if (bIsInterpolatingSpeedRecovery && GetCharacterMovement())
	{
		// 현재 속도로부터 Default 속도까지 속도 값 보간
		CurrentSpeed = FMath::FInterpTo(CurrentSpeed, DefaultMaxWalkSpeed, DeltaSeconds, SpeedRecoveryInterpSpeed);

		// 보간한 속도 값을 MaxWalkSpeed로 설정
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

		if (CurrentSpeed >= DefaultMaxWalkSpeed)
		{
			bIsInterpolatingSpeedRecovery = false;
		}
	}
}

float APFEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	CheckHP(ActualDamage);

	bIsInterpolatingSpeedRecovery = true;
	CurrentSpeed = 10.0f;

	// 실제 대미지 반환
	return ActualDamage;
}

void APFEnemy::Die()
{
	Super::Die();
	
	if (EnemyIcon)
	{
		EnemyIcon->SetVisibility(false);
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	
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
		RightHandCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	}
	else if (InSectionName == TEXT("LeftAttack"))
	{
		LeftHandCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	}
}

void APFEnemy::DisableAttackCollision(FName InSectionName)
{
	if (InSectionName == TEXT("RightAttack"))
	{
		RightHandCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}
	else if (InSectionName == TEXT("LeftAttack"))
	{
		LeftHandCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void APFEnemy::OnAttackTaskEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

void APFEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == GetOwner() || GetInstigator() == OtherActor->GetInstigator())
	{
		return;
	}
	
	APFCharacterPlayer* CharacterPlayer = Cast<APFCharacterPlayer>(OtherActor);
	if (CharacterPlayer)
	{
		UGameplayStatics::ApplyDamage(CharacterPlayer, BaseDamage, GetController(), this, UDamageType::StaticClass());

		if (AttackHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, AttackHitSound, SweepResult.ImpactPoint);
		}
		
		if (CharacterPlayer->GetIsDead())
		{
			// 플레이어가 사망한 경우 AIController의 기능 정지
			APFAIController* AIController = Cast<APFAIController>(GetController());
			AIController->StopAI();
		}
	}
}

float APFEnemy::GetAIAttackRange()
{
	return 150.0f;
}

float APFEnemy::GetAITurnSpeed()
{
	return 2.0f;
}

void APFEnemy::SetAIAttackDelegate(const FAIAttackFinishedDelegate& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APFEnemy::AttackByAI()
{
	bIsAttacking = true;

	// 0일 때 RightAttack, 1일 때 LeftAttack으로 설정하고 0과 1 중에서 임의의 숫자 설정 후 랜덤 공격
	uint8 RandomSection = FMath::RandRange(0, 1);
	FName SectionName = (RandomSection == 0) ? TEXT("RightAttack") : TEXT("LeftAttack");

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}
