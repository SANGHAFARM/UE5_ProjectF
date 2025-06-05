// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFAnimInstance.h"
#include "PFPlayerAnimInstance.generated.h"

class APFCharacterPlayer;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFPlayerAnimInstance : public UPFAnimInstance
{
	GENERATED_BODY()
	
public:
	UPFPlayerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<APFCharacterPlayer> PFCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PFCharacter)
	uint8 bIsAiming : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PFCharacter)
	uint8 bCloseToWall : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PFCharacter)
	uint8 bIsSprint : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PFCharacter)
	float JumpVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PFCharacter)
	FVector MouseSwayLocation;
};
