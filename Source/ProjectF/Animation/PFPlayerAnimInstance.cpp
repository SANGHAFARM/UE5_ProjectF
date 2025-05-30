// Fill out your copyright notice in the Description page of Project Settings.


#include "PFPlayerAnimInstance.h"

#include "ProjectF/Character/PFCharacterPlayer.h"

UPFPlayerAnimInstance::UPFPlayerAnimInstance()
{
}

void UPFPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// PFCharacter 캐스팅
	PFCharacter = Cast<APFCharacterPlayer>(Owner);
}

void UPFPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PFCharacter)
	{
		// 캐스팅한 PFCharacter로부터 값을 갱신
		bIsAiming = PFCharacter->GetIsAiming();
		bCloseToWall = PFCharacter->GetCloseToWall();
		bIsSprint = PFCharacter->GetIsSprint();
	}
}
