// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Weapon/Bullet.h"
#include "Character/PFCharacterPlayer.h"
#include "Components/ForceFeedbackComponent.h"

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
	
	CurrentAmmo = MaxAmmo;
}

void AWeaponBase::FireStart()
{
	if (GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		return;
	}
	
	bIsFiring = true;

	Fire();
}

void AWeaponBase::Fire()
{
	// 재장전 중이라면 return
	if (bIsReloading)
	{
		return;
	}
	
	// Ammo가 0이라면 재장전 실행 후 리턴
	if (CurrentAmmo <= 0)
	{
		ReloadStart();
		return;
	}
	
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

	// 현재 입력 디바이스가 게임패드일 때만 진동 설정
	if (CachedPFCharacter->CheckCurrentInputDeviceIsGamepad())
	{
		if (FireFeedbackComponent)
		{
			FireFeedbackComponent->Play();
		}
	}

	bOnCoolDown = true;

	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::FireCooldownEnd, FireRate, false);
	}
}

void AWeaponBase::FireEnd()
{
	bIsFiring = false;
	
	if (FireFeedbackComponent->IsActive())
	{
		FireFeedbackComponent->Stop();
	}
}

void AWeaponBase::ReloadStart()
{
	// 이미 재장전 중이라면 return
	if (bIsReloading)
	{
		return;
	}
	
	// 현재 Ammo가 전부 있다면 재장전을 할 필요가 없기 때문에 return
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

	if (FireFeedbackComponent->IsActive() == false)
	{
		FireFeedbackComponent->Play();
	}
	
	UpdateAmmoHUD();
}

void AWeaponBase::BulletHitEnemy(AActor* HitActor)
{
	// HitActor가 유효한지 체크
	if (HitActor)
	{
		// HitActor가 유효하면 APFCharacterBase로 캐스팅이 되는지 확인
		APFCharacterBase* PFCharacter = Cast<APFCharacterBase>(HitActor);
		if (PFCharacter)
		{
			// APFCharacterBase로 캐스팅에 성공하면 APFCharacterBase의 bIsDead 상태를 PFCharacter의 NotifyHitmarker 함수로 넘김
			if (PFCharacter->GetIsDead())
			{
				CachedPFCharacter->NotifyHitmarker(true);
			}
			else
			{
				CachedPFCharacter->NotifyHitmarker(false);
			}
		}
	}
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		CachedPFCharacter = Cast<APFCharacterPlayer>(GetOwner());
		if (CachedPFCharacter)
		{
			CachedCharacterArmsAnimInstance = CachedPFCharacter->GetCharacterArms()->GetAnimInstance();
		}
	}

	// UForceFeedbackComponent 생성 후 저장
	FireFeedbackComponent = NewObject<UForceFeedbackComponent>(this);
	FireFeedbackComponent->bAutoActivate = false;
	FireFeedbackComponent->bAutoDestroy = false;
	// 컴포넌트를 월드에 등록
	FireFeedbackComponent->RegisterComponentWithWorld(GetWorld());
	FireFeedbackComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	FireFeedbackComponent->bLooping = false;
	if (FireFeedbackEffect)
	{
		FireFeedbackComponent->SetForceFeedbackEffect(FireFeedbackEffect);
	}

	MaxAmmo = 40;
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			ABullet* SpawnedBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SocketTransform, SpawnParams);
			if (SpawnedBullet)
			{
				// Bullet의 델리게이트에 Enemy를 Hit 시 실행할 WeaponBase의 BulletHitEnemy 함수 바인딩
				SpawnedBullet->OnBulletHitEnemyDelegate.BindUObject(this, &AWeaponBase::BulletHitEnemy);
				SpawnedBullet->SetBulletDamage(WeaponDamage);
			}
		}
	}
}

void AWeaponBase::FireCooldownEnd()
{
	bOnCoolDown = false;

	if (bIsFiring)
	{
		Fire();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
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