// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PFCharacterBase.generated.h"

UCLASS()
class PROJECTF_API APFCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APFCharacterBase();

	FORCEINLINE bool GetIsDead() const { return bIsDead; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckHP(float InDamage);

	virtual void Die();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = HP)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, Category = HP)
	float MaxHP;

	uint8 bIsDead : 1 = false;
};
