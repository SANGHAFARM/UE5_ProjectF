// Fill out your copyright notice in the Description page of Project Settings.


#include "PFCharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "Enemy/PFEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/PFHUDWidget.h"
#include "Weapon/WeaponBase.h"

APFCharacterPlayer::APFCharacterPlayer()
{
	// ACharacter로부터 상속받은 Mesh는 사용하지 않을 예정이기 때문에 각종 설정을 비활성화
	GetMesh()->SetAutoActivate(false);
	GetMesh()->CastShadow = false;
	GetMesh()->SetVisibility(false);
	
	// Pivot
	// Mesh를 부착해서 Pitch 회전 시 Pivot에 따라 Mesh가 움직이도록 설정하기 위한 기준점
	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	Pivot->SetupAttachment(RootComponent);
	Pivot->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	
	// CharacterArms
	// Pivot에 붙여서 Pitch 회전에 따라 Mesh를 회전시키기 위해 사용할 진짜 Mesh
	CharacterArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterArms"));
	CharacterArms->SetupAttachment(Pivot);
	CharacterArms->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -160.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CharacterArms, TEXT("head"));
	// 카메라의 회전이 컨트롤러(폰)의 회전을 따르도록 설정
	Camera->bUsePawnControlRotation = true;
	// 카메라의 위치를 1인칭에 맞게 캐릭터의 머리 위치로 설정
	Camera->SetRelativeRotation(FRotator(0.0f, -90.0f, 90.0f));
	
	// 캐릭터는 항상 컨트롤러의 Yaw 회전 값을 따르도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// 컨트롤러의 Yaw 회전 값을 따르기 때문에
	// 캐릭터가 움직이는 방향으로 자연스럽게 회전할지 여부는 false로 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Sight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(GetRootComponent());
	SpotLight->SetMobility(EComponentMobility::Movable);
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, 1600.0f));
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	SpotLight->SetIntensity(20.0f);
	SpotLight->SetIntensityUnits(ELightUnits::Candelas);
	SpotLight->SetAttenuationRadius(3000.0f);

	SightRadius = CreateDefaultSubobject<USphereComponent>(TEXT("SightRadius"));
	SightRadius->SetupAttachment(GetRootComponent());
	SightRadius->SetMobility(EComponentMobility::Movable);
	SightRadius->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SightRadius->SetSphereRadius(1600.0f);
	SightRadius->SetCollisionProfileName(TEXT("NoCollision"));
	SightRadius->CanCharacterStepUpOn = ECB_No;

	EnemySpawnRadius = CreateDefaultSubobject<USphereComponent>(TEXT("EnemySpawnRadius"));
	EnemySpawnRadius->SetupAttachment(GetRootComponent());
	EnemySpawnRadius->SetMobility(EComponentMobility::Movable);
	EnemySpawnRadius->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	EnemySpawnRadius->SetSphereRadius(2400.0f);
	EnemySpawnRadius->SetCollisionProfileName(TEXT("NoCollision"));
	EnemySpawnRadius->CanCharacterStepUpOn = ECB_No;

	// Minimap
	MinimapArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapArm"));
	MinimapArm->SetupAttachment(GetRootComponent());
	MinimapArm->TargetArmLength = 600.0f;
	MinimapArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	MinimapArm->bDoCollisionTest = false;
	MinimapArm->bInheritPitch = false;
	MinimapArm->bInheritRoll = false;
	MinimapArm->bInheritYaw = false;
	
	MinimapSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapSceneCapture"));
	MinimapSceneCapture->SetupAttachment(MinimapArm);
	MinimapSceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	MinimapSceneCapture->OrthoWidth = 3000.0f;
	MinimapSceneCapture->ShowFlags.SetDynamicShadows(false);

	// Icon
	PlayerIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerIcon"));
	PlayerIcon->SetupAttachment(GetRootComponent());
	PlayerIcon->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	PlayerIcon->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	PlayerIcon->SetRelativeScale3D(FVector(0.2));
	PlayerIcon->SetEnableGravity(false);
	PlayerIcon->bApplyImpulseOnDamage = false;
	PlayerIcon->bReplicatePhysicsToAutonomousProxy = false;
	PlayerIcon->SetGenerateOverlapEvents(false);
	PlayerIcon->bVisibleInReflectionCaptures = false;
	PlayerIcon->bVisibleInRealTimeSkyCaptures = false;
	PlayerIcon->bVisibleInRayTracing = false;
	PlayerIcon->TranslucencySortPriority = 1;
	PlayerIcon->SetVisibleInSceneCaptureOnly(true);

	PlayerSight = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerSight"));
	PlayerSight->SetupAttachment(PlayerIcon);
	PlayerSight->SetRelativeLocation(FVector(0.0f, -150.0f, 4300.0f));
	PlayerSight->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
	PlayerSight->SetRelativeScale3D(FVector(7.0f, 1.0f, 7.0f));
	PlayerSight->SetEnableGravity(false);
	PlayerSight->bApplyImpulseOnDamage = false;
	PlayerSight->bReplicatePhysicsToAutonomousProxy = false;
	PlayerSight->SetGenerateOverlapEvents(false);
	PlayerSight->bVisibleInReflectionCaptures = false;
	PlayerSight->bVisibleInRealTimeSkyCaptures = false;
	PlayerSight->bVisibleInRayTracing = false;
	PlayerSight->TranslucencySortPriority = -1;
	PlayerSight->SetVisibleInSceneCaptureOnly(true);
	
	// 입력
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Game/ProjectF/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingContextRef.Object)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Look.IA_Look"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Sprint.IA_Sprint"));
	if (SprintActionRef.Object)
	{
		SprintAction = SprintActionRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> CrouchActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Crouch.IA_Crouch"));
	if (CrouchActionRef.Object)
	{
		CrouchAction = CrouchActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Aim.IA_Aim"));
	if (AimActionRef.Object)
	{
		AimAction = AimActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FireActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Fire.IA_Fire"));
	if (FireActionRef.Object)
	{
		FireAction = FireActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionRef(TEXT("/Game/ProjectF/Input/Actions/IA_Reload.IA_Reload"));
	if (ReloadActionRef.Object)
	{
		ReloadAction = ReloadActionRef.Object;
	}

	// 기준 달리기 속도 저장
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// 기준 FOV 값 저장
	Camera->FieldOfView = DefaultFOV;

	// 캐릭터가 앉기를 사용할 수 있는 여부 설정
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// 기준 캡슐 높이 설정
	DefaultCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	// 캐릭터가 앉았을 때 캡슐 크기를 평상시 캡슐 크기의 반으로 설정
	GetCharacterMovement()->SetCrouchedHalfHeight(DefaultCapsuleHalfHeight / 2);
	
	MaxHP = 100.0f;
}

void APFCharacterPlayer::NotifyHitmarker(bool bTargetIsDead)
{
	OnShowHitmarker.ExecuteIfBound(bTargetIsDead);
}

bool APFCharacterPlayer::CheckCurrentInputDeviceIsGamepad()
{
	if (CachedInputSubsystem)
	{
		FHardwareDeviceIdentifier CurrentDevice = CachedInputSubsystem->GetMostRecentlyUsedHardwareDevice(CachedUserId);

		if (CurrentDevice.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad)
		{
			return true;
		}

		return false;
	}

	return false;
}

void APFCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		// 캐릭터를 무기의 오너로 설정
		SpawnParams.Owner = this;
		// 인스티게이터 설정
		SpawnParams.Instigator = GetInstigator(); 

		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		// Weapon을 CharacterArms의 ik_hand_gun 소켓에 Attach
		Weapon->AttachToComponent(CharacterArms, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ik_hand_gun"));
	}

	// Character Arms의 AnimInstance를 참조 임시(Transient) 객체에 저장
	if (CachedAnimInstance == nullptr && CharacterArms->GetAnimInstance())
	{
		CachedAnimInstance = CharacterArms->GetAnimInstance();
	}
}

void APFCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (EquipMontage && CharacterArms->GetAnimInstance())
	{
		CharacterArms->GetAnimInstance()->Montage_Play(EquipMontage);
	}
	
	// 컨트롤러 가져오기
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 입력 활성화
		EnableInput(PlayerController);

		// Subsystem 가져오기
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			// 현재 Subsystem에 있는 Mapping 목록을 Clear 후 DefaultMappingContext 추가
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer)
		{
			// 현재 Local 플레이어에게 할당된 플랫폼의 Id 캐싱
			CachedUserId = LocalPlayer->GetPlatformUserId();
		}
	}

	// InputDeviceSubsystem 캐싱
	CachedInputSubsystem = Cast<UInputDeviceSubsystem>(GEngine->GetEngineSubsystem<UInputDeviceSubsystem>());
	ensure(CachedInputSubsystem);
	
	CurrentHP = MaxHP;
}

void APFCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// InputAction과 함수 바인딩
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APFCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APFCharacterPlayer::MoveEnd);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APFCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &APFCharacterPlayer::LookEnd);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APFCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APFCharacterPlayer::ToggleSprint);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APFCharacterPlayer::ToggleCrouch);
	
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APFCharacterPlayer::AimOn);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APFCharacterPlayer::AimOff);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APFCharacterPlayer::WeaponFireStart);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APFCharacterPlayer::WeaponFireEnd);

	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APFCharacterPlayer::Reload);
}

void APFCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Crouch 시 위치 보간
	if (bIsInterpolatingCrouch && CrouchCurve)
	{
		// 경과 시간을 누적하고 CrouchCurve에서 경과 시간에 일치하는 값으로 Height 값 업데이트
		CrouchElapsedTime += DeltaSeconds;
		float Alpha = FMath::Clamp(CrouchElapsedTime / CrouchDuration, 0.0f, 1.0f);

		float CurveVal = CrouchCurve->GetFloatValue(Alpha);
		float NewHeight = FMath::Lerp(StartCapsuleHalfHeight, TargetCapsuleHalfHeight, CurveVal);

		float CurrentHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		float Delta = CurrentHeight - NewHeight;

		// 현재 위치를 유지시키며 Mesh가 순간이동 또는 지면을 뚫는 것을 방지
		AddActorWorldOffset(FVector(0, 0, -Delta));
		GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight, false);

		// Alpha 값이 1에 도달하면 Crouch 보간을 중단
		if (Alpha >= 1.0f)
		{
			bIsInterpolatingCrouch = false;
		}
	}

	// Sprint 시 FOV 보간
	if (bIsInterpolatingFOV && SprintFOVCurve)
	{
		// 경과 시간을 누적하고 FOVCurve에서 경과 시간에 일치하는 값으로 FOV를 업데이트
		FOVElapsedTime += DeltaSeconds;
		float Alpha = FMath::Clamp(FOVElapsedTime / FOVTransitionDuration, 0.0f, 1.0f);
		
		float CurveVal = SprintFOVCurve->GetFloatValue(Alpha);
		float NewFOV = FMath::Lerp(StartFOV, TargetFOV, CurveVal);

		Camera->SetFieldOfView(NewFOV);

		// Alpha 값이 1에 도달하면 FOV 보간을 중단
		if (Alpha >= 1.0f)
		{
			bIsInterpolatingFOV = false;
		}
	}

	// 적으로부터 피격 시 Time Dilation 느리게 설정
	// TakeDamage 함수에서 bIsInterpolatingHitTimeDilation 값을 true로 설정
	if (bIsInterpolatingHitTimeDilation && GetWorld())
	{
		HitDilationTime = FMath::FInterpTo(HitDilationTime, 1.0f, DeltaSeconds, HitDilationInterpSpeed); // 2.0f는 보간 속도
		
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), HitDilationTime);
		
		if (HitDilationTime >= 1.0f)
		{
			bIsInterpolatingHitTimeDilation = false;
		}
	}

	if (Controller && Pivot)
	{
		// Mesh를 Controller의 Pitch와 일치
		Pivot->SetRelativeRotation(FRotator(GetControlRotation().Pitch, 0.0f, 0.0f));
	}

	if (Camera)
	{
		FHitResult HitWallResult;
		FVector Start = Camera->GetComponentLocation();
		FVector ToWall = Start + Camera->GetForwardVector() * 100.0f;
		
		// 카메라로부터 카메라 전방 벡터 방향의 End 지점 사이에 물체 유무 확인 LineTrace
		bool bHitWall = GetWorld()->LineTraceSingleByChannel(HitWallResult, Start, ToWall, ECC_Visibility);
		if (bHitWall)
		{
			// CloseToWall에서 Enemy는 예외하는 로직 추가
			if (HitWallResult.GetActor()->IsA<APFEnemy>() == false)
			{
				bCloseToWall = true;
				// bCloseToWall 상태라면 Fire 중지
				WeaponFireEnd();
			}
		}
		else
		{
			bCloseToWall = false;
		}

		FHitResult HitResult;
		FVector End = Start + Camera->GetForwardVector() * 10000.0f;
		
		bCurrentFrameOnEnemy = false;

		// 크로스헤어 색상 변경을 위한 LineTrace
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
		if (bHit)
		{
			if (HitResult.GetActor()->IsA<APFEnemy>())
			{
				bCurrentFrameOnEnemy = true;
			}
		}

		// 1. 마지막 프레임에서 Enemy를 겨냥하지 않았는데, 현재 프레임에서 Enemy를 겨냥한 경우 -> Color::Red
		// 2. 마지막 프레임에서 Enemy를 겨냥했는데, 현재 프레임에서 Enemy를 겨냥하지 않은 경우 -> Color::White
		if (bCurrentFrameOnEnemy != bLastFrameOnEnemy)
		{
			if (bCurrentFrameOnEnemy)
			{
				OnChangeCrosshairColor.ExecuteIfBound(FColor::Red);
			}
			else
			{
				OnChangeCrosshairColor.ExecuteIfBound(FColor::White);
			}

			bLastFrameOnEnemy = bCurrentFrameOnEnemy;
		}
	}

	// 플레이어로부터 Causer 위치의 방향 업데이트 함수
	UpdatePlayerToCauserAngle();
}

float APFCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (GetWorld())
	{
		// Timer에 Causer를 nullptr로 초기화시키는 함수 연결
		GetWorld()->GetTimerManager().SetTimer(ResetCauserTimerHandle, this, &APFCharacterPlayer::ResetCauser, 2.0f, false);
		// Timer에 체력 재생 함수 연결
		GetWorld()->GetTimerManager().SetTimer(HPRegenTimerHandle, this, &APFCharacterPlayer::HPRegen, 1.0f, true);

		if (DamageCauser)
		{
			Causer = DamageCauser;
			UpdatePlayerToCauserAngle();
		}
	}

	if (CheckCurrentInputDeviceIsGamepad())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			FForceFeedbackParameters ForceFeedbackParams;
			ForceFeedbackParams.bLooping = false;
			ForceFeedbackParams.bIgnoreTimeDilation = true;
			ForceFeedbackParams.bPlayWhilePaused = false;
		
			PlayerController->ClientPlayForceFeedback(GetHitFeedback, ForceFeedbackParams);
		}
	}
	
	HitDilation();
	CheckHP(Damage);

	// 바인딩 했던 HUD의 HPWidget 업데이트 함수 실행
	OnUpdateHP.ExecuteIfBound(CurrentHP, MaxHP);

	// 바인딩 했던 HUD의 IndicatorWidget의 애니메이션을 재생하는 함수 실행
	OnPlayIndicatorAnimation.ExecuteIfBound();

	return Damage;
}

void APFCharacterPlayer::SetupHUDWidget(UPFHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		if (Weapon)
		{
			// 현재 Weapon의 Ammo 값으로 업데이트
			InHUDWidget->UpdateAmmo(Weapon->GetCurrentAmmo(), Weapon->GetMaxAmmo());
			// HUD에 표시되는 Ammo 값을 업데이트 하기 위해 HUD의 함수와 바인딩
			Weapon->OnAmmoChanged.BindUObject(InHUDWidget, &UPFHUDWidget::UpdateAmmo);
		}

		// 조준/비조준 시 Crosshair 표시 여부 HUD 함수와 바인딩
		OnCrosshairSetHide.BindUObject(InHUDWidget, &UPFHUDWidget::HideCrosshair);

		// 적 조준/비조준 시 Crosshair 색상 설정 HUD 함수와 바인딩
		OnChangeCrosshairColor.BindUObject(InHUDWidget, &UPFHUDWidget::ChangeCrosshairColor);

		// 적 Hit 시 Hitmarker 표시 여부 HUD 함수와 바인딩
		OnShowHitmarker.BindUObject(InHUDWidget, &UPFHUDWidget::ShowHitmarker);

		// HP 프로그레스바를 업데이트하는 HUD 함수와 바인딩
		OnUpdateHP.BindUObject(InHUDWidget, &UPFHUDWidget::UpdateHP);

		// Indicator Angle을 업데이트하는 HUD 함수와 바인딩
		OnUpdateIndicator.BindUObject(InHUDWidget, &UPFHUDWidget::UpdateDamageDirectionIndicator);
		// Indicator Animation을 실행할 HUD 함수와 바인딩
		OnPlayIndicatorAnimation.BindUObject(InHUDWidget, &UPFHUDWidget::PlayDamageDirectionIndicatorAnimation);

		// 플레이어 사망 시 HUD 위젯을 숨기는 HUD 함수와 바인딩
		OnPlayerDeadSetHideHUD.AddUObject(InHUDWidget, &UPFHUDWidget::SetHideHUDWidget);
	}
}

void APFCharacterPlayer::Die()
{
	Super::Die();

	if (OnPlayerDeadStopAI.IsBound())
	{
		OnPlayerDeadStopAI.Broadcast();
	}

	WeaponFireEnd();

	// 캡슐 컴포넌트를 NoCollision으로 설정하여 Collision 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// CharacterArms 숨김 처리
	CharacterArms->SetVisibility(false);
	// Weapon 숨김 처리
	Weapon->SetHidden(true);

	// HUD를 숨김 처리하는 함수와 바인딩된 델리게이트 실행
	OnPlayerDeadSetHideHUD.Broadcast();
	
	// HPRegenTimerHandle Clear
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HPRegenTimerHandle);
	}

	// 컨트롤러 가져오기
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 입력 비활성화
		DisableInput(PlayerController);
	}
}

void APFCharacterPlayer::Move(const FInputActionValue& Value)
{
	// 입력 값 읽기
	FVector2D Movement = Value.Get<FVector2D>();

	// 컨트롤러 회전 값
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	
	// 방향 구하기
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// 무브먼트 컴포넌트에 값 전달
	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RightVector, Movement.Y);
}

void APFCharacterPlayer::MoveEnd()
{
	// 달리는 중이지만 이동 속도가 3.0f 미만이면 달리기 종료
	if (bIsSprint)
	{
		ToggleSprint();
	}
}

void APFCharacterPlayer::Look(const FInputActionValue& Value)
{
	// 입력 값 읽기
	MouseInput = Value.Get<FVector2D>();
	
	if (CheckCurrentInputDeviceIsGamepad())
	{
		UE_LOG(LogTemp, Warning, TEXT("Device : Gamepad"))
		// 현재 입력 디바이스가 Gamepad
		if (bIsAiming)
		{
			// Aim 중일 때 민감도를 낮게 조절
			AddControllerYawInput(MouseInput.X * GamepadSensitivityOnAim);
			AddControllerPitchInput(MouseInput.Y * GamepadSensitivityOnAim);
		}
		else
		{
			AddControllerYawInput(MouseInput.X * GamepadSensitivity);
			AddControllerPitchInput(MouseInput.Y * GamepadSensitivity);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Device : KeyboardAndMouse"));
		// 현재 입력 디바이스가 KeyboardAndMouse
		if (bIsAiming)
		{
			// Aim 중일 때 민감도를 낮게 조절
			AddControllerYawInput(MouseInput.X * MouseSensitivityOnAim);
			AddControllerPitchInput(MouseInput.Y * MouseSensitivityOnAim);
		}
		else
		{
			AddControllerYawInput(MouseInput.X * MouseSensitivity);
			AddControllerPitchInput(MouseInput.Y * MouseSensitivity);
		}
	}
}

void APFCharacterPlayer::LookEnd(const FInputActionValue& Value)
{
	// 입력 값 읽기
	MouseInput = Value.Get<FVector2D>();
}

void APFCharacterPlayer::Jump()
{
	Super::Jump();

	// 달리기 중에 점프를 하면 달리기 취소
	if (bIsSprint)
	{
		ToggleSprint();
	}
}

void APFCharacterPlayer::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
		SetCrouch(false);
	}
	else
	{
		if (CanCrouch())
		{
			Crouch();
			SetCrouch(true);
		}
	}
}

void APFCharacterPlayer::ToggleSprint()
{
	if (bIsSprint == false && GetCharacterMovement() && GetCharacterMovement()->Velocity.Size2D() < 3.0f)
	{
		return;
	}
	
	bIsSprint = !bIsSprint;
	
	// true면 달리기
	if (bIsSprint)
	{
		SprintOn();
	}
	// false면 걷기
	else
	{
		SprintOff();
	}
}

void APFCharacterPlayer::AimOn()
{
	bIsAiming = true;

	// 달리기 상태라면 달리기 취소
	if (bIsSprint)
	{
		ToggleSprint();
	}

	// OnCrosshairSetHide 델리게이트와 바인딩 된 함수에 true를 전달하여 크로스헤어 감추도록 설정
	OnCrosshairSetHide.ExecuteIfBound(true);
}

void APFCharacterPlayer::AimOff()
{
	bIsAiming = false;

	// OnCrosshairSetHide 델리게이트와 바인딩 된 함수에 false를 전달하여 크로스헤어 보이도록 설정
	OnCrosshairSetHide.ExecuteIfBound(false);
}

void APFCharacterPlayer::WeaponFireStart()
{
	if (Weapon && CanFire())
	{
		Weapon->Fire();
	}
}

void APFCharacterPlayer::WeaponFireEnd()
{
	if (Weapon)
	{
		Weapon->FireEnd();
	}
}

bool APFCharacterPlayer::CanFire() const
{
	// 벽에 붙어 있거나 달리는 중이라면 return false
	if (bCloseToWall || bIsSprint)
	{
		return false;
	}
	
	return true;
}

void APFCharacterPlayer::Reload()
{
	if (Weapon)
	{
		Weapon->ReloadStart();
	}
}

void APFCharacterPlayer::UpdatePlayerToCauserAngle()
{
	if (Causer == nullptr)
	{
		return;
	}
	
	// 현재 플레이어의 위치를 기준으로 Causer가 위치한 방향을 구해서 전달
	FVector PlayerLocation = GetActorLocation();
	FVector CauserLocation = Causer->GetActorLocation();
	FRotator PlayerControlRotation = GetControlRotation();
	float Angle = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, CauserLocation).Yaw - PlayerControlRotation.Yaw;
	
	OnUpdateIndicator.ExecuteIfBound(Angle);
}

void APFCharacterPlayer::ResetCauser()
{
	if (GetWorld() && Causer)
	{
		GetWorld()->GetTimerManager().ClearTimer(ResetCauserTimerHandle);
		Causer = nullptr;
	}
}

void APFCharacterPlayer::HPRegen()
{
	if (CurrentHP < MaxHP)
	{
		float NewHP = CurrentHP + 2.0f;
		CurrentHP = FMath::Clamp(NewHP, 0.0f, MaxHP);

		OnUpdateHP.ExecuteIfBound(CurrentHP, MaxHP);
	}
	else
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(HPRegenTimerHandle);
		}
	}
}

void APFCharacterPlayer::HitDilation()
{
	HitDilationTime = 0.2f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), HitDilationTime);
	
	bIsInterpolatingHitTimeDilation = true;
}

void APFCharacterPlayer::SetFOV(const float InTargetFOV)
{
	if (Camera == nullptr)
	{
		return;
	}
	
	// 현재 FOV 값과 목표 FOV 값을 설정
	StartFOV = Camera->FieldOfView;
	TargetFOV = InTargetFOV;
	FOVElapsedTime = 0.0f;
	bIsInterpolatingFOV = true;
}

void APFCharacterPlayer::SprintOn()
{
	SetFOV(SprintFOV);

	// 현재 앉기 상태였다면 앉기 취소
	if (bIsCrouched)
	{
		ToggleCrouch();
	}

	// 현재 Fire 중이었다면 Fire 중지  
	if (Weapon->GetIsFiring())
	{
		WeaponFireEnd();
	}
	
	// 달리기 속도 2배로 설정
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed * 2;
}

void APFCharacterPlayer::SprintOff()
{
	SetFOV(DefaultFOV);

	// 달리기 속도 기본으로 설정
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void APFCharacterPlayer::SetCrouch(bool bSetCrouch)
{
	bIsInterpolatingCrouch = true;
	CrouchElapsedTime = 0.0f;

	StartCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	if (bSetCrouch)
	{
		TargetCapsuleHalfHeight = GetCharacterMovement()->GetCrouchedHalfHeight();
	}
	else
	{
		TargetCapsuleHalfHeight = DefaultCapsuleHalfHeight;
	}
}

void APFCharacterPlayer::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);

	// 달리기 중에 앉으면 달리기 취소
	if (bIsSprint)
	{
		ToggleSprint();
	}
}

void APFCharacterPlayer::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);
}