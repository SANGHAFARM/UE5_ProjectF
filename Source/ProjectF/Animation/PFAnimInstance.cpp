// Fill out your copyright notice in the Description page of Project Settings.


#include "PFAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPFAnimInstance::UPFAnimInstance()
{
	// Idle 상태를 정하는 속도 기준
	MovingThreshold = 3.0f;
	// Jump 상태를 정하는 속도 기준
	JumpingThreshold = 100.0f;
}

void UPFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// AnimInstance를 소유중인 Actor 불러오기
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsCrouching = Movement->IsCrouching();
		bIsJumping = bIsFalling && (Velocity.Z > JumpingThreshold);
	}
}