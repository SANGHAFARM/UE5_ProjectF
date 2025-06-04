// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	MaxAmmo = 40;
	CurrentAmmo = MaxAmmo;
}

void AWeaponBase::ConsumeBullet()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}
	else
	{
		CurrentAmmo = 0;
	}

	// OnAmmoChanged 델리게이트와 바인딩 된 함수 실행
	OnAmmoChanged.ExecuteIfBound(CurrentAmmo, MaxAmmo);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

