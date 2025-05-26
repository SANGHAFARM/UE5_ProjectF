// Fill out your copyright notice in the Description page of Project Settings.


#include "PFCharacterBase.h"

#include "Components/CapsuleComponent.h"

// Sets default values
APFCharacterBase::APFCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	// 메시 콜리전 및 위치, 회전 설정
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
}

// Called when the game starts or when spawned
void APFCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APFCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APFCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

