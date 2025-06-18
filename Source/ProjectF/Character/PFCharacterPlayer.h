// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PFCharacterBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Interface/PFCharacterHUDInterface.h"
#include "PFCharacterPlayer.generated.h"

DECLARE_DELEGATE_OneParam(FOnSetHideCrosshairDelegate, bool /* bSetHide */);
DECLARE_DELEGATE_OneParam(FOnChangeCrosshairColorDelegate, FLinearColor /* InColor */);
DECLARE_DELEGATE_OneParam(FOnShowHitmarkerDelegate, bool /* bTargetIsDead */);
DECLARE_DELEGATE_TwoParams(FOnUpdateHPDelegate, float /* NewCurrentHP */, float /* MaxHP */);
DECLARE_DELEGATE_OneParam(FOnUpdateIndicatorAngleDelegate, float /* NewAngle */);
DECLARE_DELEGATE(FOnPlayIndicatorAnimationDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayerDead);


class USpringArmComponent;
class AWeaponBase;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class ICrosshairInterface;
class USpotLightComponent;
class USphereComponent;
class UPaperSpriteComponent;
/**
 * 
 */
UCLASS()
class PROJECTF_API APFCharacterPlayer : public APFCharacterBase, public IPFCharacterHUDInterface
{
	GENERATED_BODY()

public:
	APFCharacterPlayer();

	// ICrosshairInterface를 통해 HUDWidget에 Hitmarker 정보를 알릴 함수
	void NotifyHitmarker(bool bTargetIsDead);

	// 현재 입력 디바이스가 게임패드인지 확인
	bool CheckCurrentInputDeviceIsGamepad();

	// Getter
public:
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	FORCEINLINE bool GetCloseToWall() const { return bCloseToWall; }
	FORCEINLINE bool GetIsSprint() const { return bIsSprint; }
	FORCEINLINE FVector2D GetMouseInput() const { return MouseInput; }
	FORCEINLINE USkeletalMeshComponent* GetCharacterArms() const { return CharacterArms; }
	FORCEINLINE float GetSightRadius() const { return SightRadius->GetScaledSphereRadius(); }
	FORCEINLINE float GetEnemySpawnRadius() const { return EnemySpawnRadius->GetScaledSphereRadius(); }
	
protected:
	// BeginPlay보다 이전에 실행되는 초기화 함수
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// IPFCharacterHUDInterface 함수 구현
	virtual void SetupHUDWidget(UPFHUDWidget* InHUDWidget) override;

	virtual void Die() override;
	
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	FOnPlayerDead OnPlayerDeadStopAI;

	// Delegate
protected:
	// Crosshair 관련
	FOnSetHideCrosshairDelegate OnCrosshairSetHide;
	
	FOnChangeCrosshairColorDelegate OnChangeCrosshairColor;
	uint8 bCurrentFrameOnEnemy : 1 = false;
	uint8 bLastFrameOnEnemy : 1 = false;

	FOnShowHitmarkerDelegate OnShowHitmarker;

	// Damage Direction Indicator 관련
	FOnUpdateIndicatorAngleDelegate OnUpdateIndicator;
	FOnPlayIndicatorAnimationDelegate OnPlayIndicatorAnimation;

	// HP 관련
	FOnUpdateHPDelegate OnUpdateHP;

	// 플레이어 사망 시 HUD 숨김 처리 함수와 바인딩 할 델리게이트
	FOnPlayerDead OnPlayerDeadSetHideHUD;
	
	// 조작
protected:
	// Look 함수에서 Cast 부하를 줄이기 위해 InputDeviceSubsystem 캐싱
	UPROPERTY(Transient)
	TObjectPtr<UInputDeviceSubsystem> CachedInputSubsystem;

	// Look 함수에서 Cast 부하를 줄이기 위해 GetMostRecentlyUsedHardwareDevice 함수에 사용할 PlatformUserId 캐싱
	UPROPERTY(Transient)
	FPlatformUserId CachedUserId;

	// 비조준 시 마우스 민감도
	UPROPERTY(EditAnywhere, Category = Input)
	float MouseSensitivity = 0.8f;

	// 조준 시 마우스 민감도
	UPROPERTY(EditAnywhere, Category = Input)
	float MouseSensitivityOnAim = 0.4f;

	// 비조준 시 게임패드 민감도
	UPROPERTY(EditAnywhere, Category = Input)
	float GamepadSensitivity = 0.8f;

	// 조준 시 게임패드 민감도
	UPROPERTY(EditAnywhere, Category = Input)
	float GamepadSensitivityOnAim = 0.3f;
	
	// 애니메이션에 쓰기 위해 마우스 입력 값 저장
	FVector2D MouseInput;
	
	void Move(const FInputActionValue& Value);
	void MoveEnd();
	void Look(const FInputActionValue& Value);
	// AnimInstance에 Mouse 값을 넘겨주기 위해 LookAction에 바인딩할 Complete 함수
	void LookEnd(const FInputActionValue& Value);
	virtual void Jump() override;
	
	// 오버라이드된 Crouch()와 UnCrouch() 함수의 래핑 함수
	void ToggleCrouch();
	void ToggleSprint();
	
	void AimOn();
	void AimOff();
	
	void WeaponFireStart();
	void WeaponFireEnd();
	bool CanFire() const;

	void Reload();

	void UpdatePlayerToCauserAngle();
	void ResetCauser();

	// HP
	FTimerHandle HPRegenTimerHandle;
	void HPRegen();

	// 캐릭터
protected:
	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> CharacterArms;

	// 캐릭터 Mesh 회전의 기준 축이 될 Pivot
	UPROPERTY(EditAnywhere, Category = Pivot)
	TObjectPtr<USceneComponent> Pivot;

	// AnimInstance를 참조하는 임시 객체
	UPROPERTY(Transient)
	TObjectPtr<UAnimInstance> CachedAnimInstance;

	// Indicator에 나타나는 임시 공격자
	UPROPERTY(Transient)
	TObjectPtr<AActor> Causer = nullptr;

	FTimerHandle ResetCauserTimerHandle;

	// 피격
protected:
	UPROPERTY(EditAnywhere, Category = HitDilation)
	float HitDilationInterpSpeed = 1.0f;
	
	float HitDilationTime;
	bool bIsInterpolatingHitTimeDilation : 1 = false;
	
	void HitDilation();

	UPROPERTY(EditDefaultsOnly, Category = ForceFeedback)
	TObjectPtr<UForceFeedbackEffect> GetHitFeedback;
	
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

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ReloadAction;
	
	// 달리기
protected:
	UPROPERTY(BlueprintReadOnly, Category = Sprint)
	uint8 bIsSprint : 1 = false;

	UPROPERTY(EditAnywhere, Category = Sprint)
	TObjectPtr<UCurveFloat> SprintFOVCurve;

	float DefaultMaxWalkSpeed;

	void SprintOn();
	void SprintOff();

	// 앉기
	UPROPERTY(EditAnywhere, Category = Crouch)
	TObjectPtr<UCurveFloat> CrouchCurve;

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
	TObjectPtr<UAnimMontage> EquipMontage;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeaponBase> WeaponClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<AWeaponBase> Weapon = nullptr;

	uint8 bIsAiming : 1 = false;

	// 시야
protected:
	UPROPERTY(EditAnywhere, Category = Sight)
	TObjectPtr<USpotLightComponent> SpotLight;

	UPROPERTY(EditAnywhere, Category = Sight)
	TObjectPtr<USphereComponent> SightRadius;

	UPROPERTY(EditAnywhere, Category = Sight)
	TObjectPtr<USphereComponent> EnemySpawnRadius;

	// 미니맵
protected:
	UPROPERTY(EditAnywhere, Category = Minimap)
	TObjectPtr<USpringArmComponent> MinimapArm; 

	UPROPERTY(EditAnywhere, Category = Minimap)
	TObjectPtr<USceneCaptureComponent2D> MinimapSceneCapture;
	
	// 아이콘
protected:
	UPROPERTY(EditAnywhere, Category = Icon)
	TObjectPtr<UPaperSpriteComponent> PlayerIcon;

	UPROPERTY(EditAnywhere, Category = Icon)
	TObjectPtr<UPaperSpriteComponent> PlayerSight;
};
