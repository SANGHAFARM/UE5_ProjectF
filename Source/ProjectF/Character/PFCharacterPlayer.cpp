// Fill out your copyright notice in the Description page of Project Settings.


#include "PFCharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"

APFCharacterPlayer::APFCharacterPlayer()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// Mesh Component를 부모로 설정하고, 부모 Mesh의 "SOCKET_Camera" 소켓에 카메라 부착
	Camera->SetupAttachment(GetMesh(), TEXT("SOCKET_Camera"));
	// 카메라의 회전이 컨트롤러(폰)의 회전을 따르도록 설정
	Camera->bUsePawnControlRotation = true;
	// 카메라의 위치를 1인칭에 맞게 캐릭터의 머리 위치로 설정
	//Camera->SetRelativeLocation(FVector(10.0f, 0.0f, 80.0f));

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
}

void APFCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

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
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APFCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
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

void APFCharacterPlayer::Look(const FInputActionValue& Value)
{
	// 입력 값 읽기
	FVector2D LookVector = Value.Get<FVector2D>();
	
	// 컨트롤러에 회전 추가
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}
