// Fill out your copyright notice in the Description page of Project Settings.


#include "PFCharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectF/UI/PFHUDWidget.h"
#include "ProjectF/Weapon/WeaponBase.h"

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
	CharacterArms->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -160.0f), FRotator(0.0f, 0.0f, -90.0f));
	
	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CharacterArms, TEXT("head"));
	// 카메라의 회전이 컨트롤러(폰)의 회전을 따르도록 설정
	Camera->bUsePawnControlRotation = true;
	// 카메라의 위치를 1인칭에 맞게 캐릭터의 머리 위치로 설정
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 90.0f));
	
	// 캐릭터는 항상 컨트롤러의 Yaw 회전 값을 따르도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// 컨트롤러의 Yaw 회전 값을 따르기 때문에
	// 캐릭터가 움직이는 방향으로 자연스럽게 회전할지 여부는 false로 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
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
	}
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

	if (Controller && Pivot)
	{
		// Mesh를 Controller의 Pitch와 일치
		Pivot->SetRelativeRotation(FRotator(GetControlRotation().Pitch, 0.0f, 0.0f));
	}

	if (Camera)
	{
		FHitResult HitResult;
		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + Camera->GetForwardVector() * 100.0f;
		
		// 카메라로부터 카메라 전방 벡터 방향의 End 지점 사이에 물체 유무 확인 LineTrace
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
		if (bHit)
		{
			bCloseToWall = true;
			// bCloseToWall 상태라면 Fire 중지
			WeaponFireEnd();
		}
		else
		{
			bCloseToWall = false;
		}
	}
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

			// 조준 시 실행되는 HUD 함수와 바인딩
			OnAimOn.BindUObject(InHUDWidget, &UPFHUDWidget::HideCrosshair, true);
			// 조준 해제 시 실행되는 HUD 함수와 바인딩
			OnAimOff.BindUObject(InHUDWidget, &UPFHUDWidget::HideCrosshair, false);
		}
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
	if (bIsSprint && GetCharacterMovement() && GetCharacterMovement()->Velocity.Size2D() < 3.0f)
	{
		ToggleSprint();
	}
}

void APFCharacterPlayer::Look(const FInputActionValue& Value)
{
	// 입력 값 읽기
	MouseInput = Value.Get<FVector2D>();
	
	// 컨트롤러에 회전 추가
	AddControllerYawInput(MouseInput.X);
	AddControllerPitchInput(MouseInput.Y);
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

	// OnAimOn 델리게이트와 바인딩 된 함수 실행
	OnAimOn.ExecuteIfBound();
}

void APFCharacterPlayer::AimOff()
{
	bIsAiming = false;

	// OnAimOff 델리게이트와 바인딩 된 함수 실행
	OnAimOff.ExecuteIfBound();
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

void APFCharacterPlayer::SetFOV(const float InTargetFOV)
{
	if (Camera)
	{
		// 현재 FOV 값과 목표 FOV 값을 설정
		StartFOV = Camera->FieldOfView;
		TargetFOV = InTargetFOV;
		FOVElapsedTime = 0.0f;
		bIsInterpolatingFOV = true;
	}
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