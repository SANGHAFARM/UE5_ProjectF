// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

DECLARE_DELEGATE_OneParam(FOnBulletHitEnemyDelegate, AActor* /* HitActor */)

class UProjectileMovementComponent;

UCLASS()
class PROJECTF_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	// WeaponBase의 함수와 바인딩 될 Delegate
	FOnBulletHitEnemyDelegate OnBulletHitEnemyDelegate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = Bullet)
	TObjectPtr<UStaticMeshComponent> BulletMesh;

	UPROPERTY(EditAnywhere, Category = Bullet)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = Bullet)
	TObjectPtr<UParticleSystem> HitEffect;
};
