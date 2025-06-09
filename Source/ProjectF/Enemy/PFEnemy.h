// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectF/Character/PFCharacterBase.h"
#include "PFEnemy.generated.h"

struct FDamageEvent;
class AController;
/**
 * 
 */
UCLASS()
class PROJECTF_API APFEnemy : public APFCharacterBase
{
	GENERATED_BODY()

public:
	APFEnemy();

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();
	
protected:
	UPROPERTY(EditAnywhere, Category = Enemy)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, Category = Enemy)
	float MaxHP;

protected:
	FTimerHandle DeathTimerHandle;
	float DeathTime = 3.0f;
};
