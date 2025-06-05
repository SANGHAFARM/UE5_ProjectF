// Fill out your copyright notice in the Description page of Project Settings.


#include "PFPlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
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

	if (PFCharacter.IsValid())
	{
		// 캐스팅한 PFCharacter로부터 값을 갱신
		bIsAiming = PFCharacter->GetIsAiming();
		bCloseToWall = PFCharacter->GetCloseToWall();
		bIsSprint = PFCharacter->GetIsSprint();
		
		// 캐릭터 무기의 상하좌우 흔들림을 위해 화면 좌표값으로부터 변환
		// MouseSwayLocation의 X 값을 캐릭터 MouseInput의 X 값으로 보간
		// MouseSwayLocation의 Y 값을 캐릭터 MouseInput의 Z 값으로 보간
		float AimAlpha = bIsAiming ? 0.15f : 1.0f;
		FVector TargetVec = FVector(PFCharacter->GetMouseInput().X, 0.0f, PFCharacter->GetMouseInput().Y) * AimAlpha;
		MouseSwayLocation = FMath::VInterpTo(MouseSwayLocation, TargetVec, DeltaSeconds, 8.0f);
	}

	if (Movement)
	{
		// 조준 중일 때는 점프 시 조금만 흔들리도록 Alpha 값을 조정
		float JumpAlpha = bIsAiming ? 0.3f : 1.0f;
		float TargetVal = FMath::Clamp((Movement->Velocity.Z) * 0.1f, -20.0f, 20.0f) * JumpAlpha;
		
		JumpVelocity = FMath::FInterpTo(JumpVelocity, TargetVal, DeltaSeconds, 5.0f);
	}
}