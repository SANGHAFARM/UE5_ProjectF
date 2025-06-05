// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_DELEGATE_TwoParams(FOnAmmoChanged, uint32, uint32);

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
	FORCEINLINE UAnimMontage* GetWeaponMontage() const { return WeaponMontage; }

	void ConsumeBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	uint32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Weapon)
	uint32 MaxAmmo;

	// 무기별로 재생할 무기 AnimMontage
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UAnimMontage> WeaponMontage;
};
