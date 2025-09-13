// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/ObjectPoolInterface.h"
#include "Interface/PFEnemyAIInterface.h"
#include "Interface/PFEnemyAnimationInterface.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "PFEnemy.generated.h"

class UPaperSpriteComponent;
class USphereComponent;
struct FDamageEvent;
class AController;

DECLARE_DELEGATE(FOnEnemyDiedDelegate);

/**
 * 
 */
UCLASS()
class PROJECTF_API APFEnemy : public APFCharacterBase, public IPFEnemyAIInterface, public IPFEnemyAnimationInterface, public IObjectPoolInterface
{
	GENERATED_BODY()

public:
	APFEnemy();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// ObjectPool
public:
	virtual void PooledObjectSetActive_Implementation(bool IsActive) override;
	virtual void PooledObjectDeactivate_Implementation() override;
	virtual void SetPoolIndex_Implementation(int32 Index) override;
	virtual int32 GetPoolIndex_Implementation() const override;
	virtual bool IsPoolActive_Implementation() const override;
	virtual void SetPooledObjectLifeSpan_Implementation(float LifeTime) override;

	FOnEnemyDiedDelegate OnEnemyDied;

protected:
	void ToggleActivation(bool IsActive);
	
	uint8 bIsActive : 1 = false;
	float LifeSpan = 0.0f;
	int PoolIndex = -1;

	FTimerHandle LifeSpanTimer;

protected:
	virtual void Die() override;

	// IPFEnemyAnimationInterface에서 오버라이딩한 가상 함수
	virtual void EnableAttackCollision(FName InSectionName) override;
	virtual void DisableAttackCollision(FName InSectionName) override;
	virtual void OnAttackTaskEnd() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// 피격 경직
protected:
	UPROPERTY(EditAnywhere, Category = SpeedRecovery)
	float SpeedRecoveryInterpSpeed = 1.0f;

	float CurrentSpeed;
	float DefaultMaxWalkSpeed;
	uint8 bIsInterpolatingSpeedRecovery : 1 = false;

	// 공격 관련
protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, Category = Attack)
	UAnimMontage* AttackMontage;

	uint8 bIsAttacking : 1 = false;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<USphereComponent> RightHandCollision;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<USphereComponent> LeftHandCollision;

	// AI 
protected:
	// IPFEnemyAIInterface에서 오버라이딩한 가상 함수들
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAIAttackFinishedDelegate& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAIAttackFinishedDelegate OnAttackFinished;

	// 아이콘
protected:
	UPROPERTY(EditAnywhere, Category = Icon)
	TObjectPtr<UPaperSpriteComponent> EnemyIcon;

	// 사운드
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<UAudioComponent> LocomotionSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundCue> DeathSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundCue> AttackHitSound;
};
