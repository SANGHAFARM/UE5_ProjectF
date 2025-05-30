// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PFCharacterBase.h"
#include "Components/TimelineComponent.h"
#include "PFCharacterPlayer.generated.h"

class AWeaponBase;
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

	// Getter
public:
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	FORCEINLINE bool GetCloseToWall() const { return bCloseToWall; }
	FORCEINLINE bool GetIsSprint() const { return bIsSprint; }

	// 조작
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	// 오버라이드된 Crouch()와 UnCrouch() 함수의 래핑 함수
	void ToggleCrouch();
	void ToggleSprint();
	void AimOn();
	void AimOff();

	// 캐릭터
protected:
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* CharacterArms;

	// 캐릭터 Mesh 회전의 기준 축이 될 Pivot
	UPROPERTY(EditAnywhere, Category = Pivot)
	USceneComponent* Pivot;

	// 카메라
protected:
	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

	// 카메라가 벽과 가까운지 여부
	uint8 bCloseToWall : 1 = false;

	// 매개변수 값으로 FOV를 설정하는 함수
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

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimAction;
	
	// 달리기
protected:
	UPROPERTY(BlueprintReadOnly, Category = Sprint)
	uint8 bIsSprint : 1 = false;

	UPROPERTY(EditAnywhere, Category = Sprint)
	UCurveFloat* SprintFOVCurve;

	float DefaultMaxWalkSpeed;

	void SprintOn();
	void SprintOff();

	// 앉기
	UPROPERTY(EditAnywhere, Category = Crouch)
	UCurveFloat* CrouchCurve;

	UPROPERTY(EditAnywhere, Category = Crouch)
	float CrouchDuration = 0.3f;

	float CrouchElapsedTime = 0.0f;
	uint8 bIsInterpolatingCrouch : 1 = false;

	float DefaultCapsuleHalfHeight;
	float StartCapsuleHalfHeight;
	float TargetCapsuleHalfHeight;
	
	void SetCrouch(bool bSetCrouch);
	
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;

	// 무기
protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<AWeaponBase> Weapon = nullptr;

	uint8 bIsAiming : 1 = false;
};
