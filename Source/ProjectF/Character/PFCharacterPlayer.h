// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PFCharacterBase.h"
#include "Components/TimelineComponent.h"
#include "PFCharacterPlayer.generated.h"

class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class PROJECTF_API APFCharacterPlayer : public APFCharacterBase
{
	GENERATED_BODY()

public:
	APFCharacterPlayer();
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	// 조작
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	// 오버라이드된 Crouch()와 UnCrouch() 함수의 래핑 함수
	void ToggleCrouch();
	void ToggleSprint();

	// 카메라
protected:
	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

	// 매개변수 값으로 FOV 설정 함수
	void SetFOV(const float InTargetFOV);

	// Field Of View
	FTimerHandle FOVTimerHandle;
	uint8 bIsInterpolatingFOV : 1 = false;
	float FOVTransitionDuration = 0.3f;
	float FOVElapsedTime = 0.0f;

	float DefaultFOV = 90.0f;
	float SprintFOV = 100.0f;
	float StartFOV;
	float TargetFOV;
	
	// 입력
protected:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CrouchAction;
	
	// 달리기
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Sprint")
	uint8 bIsSprint : 1 = false;

	float DefaultMaxWalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	UCurveFloat* SprintFOVCurve;

	void SprintOn();
	void SprintOff();

	// 앉기
	UPROPERTY(EditAnywhere, Category = "Crouch")
	UCurveFloat* CrouchCurve;

	UPROPERTY(EditAnywhere, Category = "Crouch")
	float CrouchDuration = 0.3f;

	float CrouchElapsedTime = 0.0f;
	uint8 bIsInterpolatingCrouch : 1 = false;

	float DefaultCapsuleHalfHeight;
	float StartCapsuleHalfHeight;
	float TargetCapsuleHalfHeight;
	
	void SetCrouch(bool bSetCrouch);
	
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;
};
