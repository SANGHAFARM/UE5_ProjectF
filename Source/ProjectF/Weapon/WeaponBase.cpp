// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectF/Weapon/Bullet.h"
#include "ProjectF/Character/PFCharacterPlayer.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMesh"));
	MagazineMesh->SetupAttachment(WeaponMesh, TEXT("Magazine"));

	MagazineMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	MaxAmmo = 40;
	CurrentAmmo = MaxAmmo;
}

void AWeaponBase::Fire()
{
	if (bIsReloading)
	{
		return;
	}
	
	if (CurrentAmmo <= 0)
	{
		ReloadStart();
		return;
	}

	bIsFiring = true;
	
	ConsumeAmmo();
	SpawnBullet();

	// 총구 효과 생성
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMesh, TEXT("MuzzleSocket"), FVector::ZeroVector, FRotator::ZeroRotator, FVector(0.03f), EAttachLocation::KeepRelativeOffset);
	}

	if (CachedCharacterArmsAnimInstance && FireMontage)
	{
		CachedCharacterArmsAnimInstance->Montage_Play(FireMontage);
	}

	if (GetWorld())
	{
		// Timer를 통해서 FireRate 시간마마다 총을 발사
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::Fire, FireRate, true);
	}
}

void AWeaponBase::FireEnd()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AWeaponBase::ReloadStart()
{
	if (CurrentAmmo == MaxAmmo)
	{
		return;
	}

	bIsReloading = true;
	
	if (CachedCharacterArmsAnimInstance)
	{
		CachedCharacterArmsAnimInstance->Montage_Play(CharacterReloadMontage);
	}

	if (WeaponMesh->GetAnimInstance())
	{
		WeaponMesh->GetAnimInstance()->Montage_Play(WeaponReloadMontage);
	}

	FTimerHandle ReloadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::ReloadEnd, 2.8f, false);
}

void AWeaponBase::ReloadEnd()
{
	bIsReloading = false;
	
	CurrentAmmo = MaxAmmo;

	UpdateAmmoHUD();
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	APFCharacterPlayer* PFCharacter = Cast<APFCharacterPlayer>(GetOwner());
	if (PFCharacter)
	{
		CachedCharacterArmsAnimInstance = PFCharacter->GetCharacterArms()->GetAnimInstance();
	}

	MaxAmmo = 40;
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFiring)
	{
		
	}
}

void AWeaponBase::ConsumeAmmo()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}
	else
	{
		CurrentAmmo = 0;
	}

	UpdateAmmoHUD();
}

void AWeaponBase::SpawnBullet()
{
	if (WeaponMesh && GetOwner())
	{
		// MuzzleSocket 위치와 회전값 가져오기
		FTransform SocketTransform = WeaponMesh->GetSocketTransform(TEXT("MuzzleSocket"));
		// MuzzleSocket의 전방 벡터 가져오기
		//FVector SpawnDirection = SocketRotation.Vector();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetOwner()->GetInstigator();

		// Bullet 생성
		if (GetWorld() && BulletClass)
		{
			GetWorld()->SpawnActor<ABullet>(BulletClass, SocketTransform, SpawnParams);
		}
	}
}

void AWeaponBase::UpdateAmmoHUD()
{
	// OnAmmoChanged 델리게이트에 바인딩 된 함수가 유효한지 확인
	if (OnAmmoChanged.IsBound())
	{
		// OnAmmoChanged 델리게이트와 바인딩 된 함수 실행
		OnAmmoChanged.Execute(CurrentAmmo, MaxAmmo);
	}
}