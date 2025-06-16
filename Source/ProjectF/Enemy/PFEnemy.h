// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PFEnemyAIInterface.h"
#include "Interface/PFEnemyAnimationInterface.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "PFEnemy.generated.h"

class UPaperSpriteComponent;
class USphereComponent;
struct FDamageEvent;
class AController;
/**
 * 
 */
UCLASS()
class PROJECTF_API APFEnemy : public APFCharacterBase, public IPFEnemyAIInterface, public IPFEnemyAnimationInterface
{
	GENERATED_BODY()

public:
	APFEnemy();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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
	
	FTimerHandle DeathTimerHandle;
	float DeathTime = 3.0f;

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
};
