// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class APFCharacterPlayer;
class ABullet;
DECLARE_DELEGATE_TwoParams(FOnAmmoChanged, uint32 /* CurrentAmmo */, uint32 /* MaxAmmo */);

UCLASS()
class PROJECTF_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	FOnAmmoChanged OnAmmoChanged;
	
public:
	FORCEINLINE uint32 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE uint32 GetMaxAmmo() const { return CurrentAmmo; }
	FORCEINLINE bool GetIsFiring() const { return bIsFiring; }
	FORCEINLINE bool GetIsReloading() const { return bIsFiring; }

	void Fire();
	void FireEnd();

	void ReloadStart();
	void ReloadEnd();

	// Bullet에 바인딩 할 Delegate 함수
	void BulletHitEnemy(AActor* HitActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// PFCharacterPlayer를 참조하는 임시 객체
	UPROPERTY(Transient)
	TObjectPtr<APFCharacterPlayer> CachedPFCharacter;
	
	// CharacterArms의 AnimInstance를 참조하는 임시 객체
	UPROPERTY(Transient)
	TObjectPtr<UAnimInstance> CachedCharacterArmsAnimInstance;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UStaticMeshComponent> MagazineMesh;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	uint32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	uint32 MaxAmmo;

	// AnimMontage
protected:
	// 무기별로 재생할 무기 AnimMontage
	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> FireMontage;

	// 캐릭터에서 재생되는 Reload 몽타주
	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> CharacterReloadMontage;

	// 무기에서 재생되는 Reload 몽타주
	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> WeaponReloadMontage;
	
	// Fire
protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<ABullet> BulletClass;

	// 연사 속도 관리
	FTimerHandle FireTimerHandle;
	float FireRate = 0.2f;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UParticleSystem> MuzzleFlash;

protected:
	void ConsumeAmmo();
	void SpawnBullet();
	
	// 델리게이트로 AmmoWidget의 Ammo 텍스트를 업데이트하는 함수
	void UpdateAmmoHUD();

	// bool
protected:
	// 발사 여부
	uint8 bIsFiring : 1 = false;
	// 재장전 여부
	uint8 bIsReloading : 1 = false;
};
