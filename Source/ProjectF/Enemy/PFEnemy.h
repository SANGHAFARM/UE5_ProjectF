// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PFEnemyAIInterface.h"
#include "Interface/PFEnemyAnimationInterface.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "PFEnemy.generated.h"

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

	bool GetIsDead() const { return bIsDead; }
	
protected:
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void Die();

	// IPFEnemyAnimationInterface에서 오버라이딩한 가상 함수
	virtual void EnableAttackCollision(FName InSectionName) override;
	virtual void DisableAttackCollision(FName InSectionName) override;
	virtual void OnAttackTaskEnd() override;
	
protected:
	UPROPERTY(EditAnywhere, Category = Enemy)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, Category = Enemy)
	float MaxHP;

protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	UAnimMontage* AttackMontage;
	
	FTimerHandle DeathTimerHandle;
	float DeathTime = 3.0f;

	uint8 bIsDead : 1 = false;

	uint8 bIsAttacking : 1 = false;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<USphereComponent> RightHandCollision;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<USphereComponent> LeftHandCollision;

	// AI
protected:
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAIAttackFinishedDelegate& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAIAttackFinishedDelegate OnAttackFinished;
};
