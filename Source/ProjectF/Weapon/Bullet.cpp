// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	SetRootComponent(BulletMesh);

	// 에디터에서 정의한 Collision 프로필로 설정
	BulletMesh->SetCollisionProfileName(TEXT("Bullet")); // <-- Bullet 프로파일 설정
	// OnHit 이벤트를 실행하기 위해 활성화
	BulletMesh->SetNotifyRigidBodyCollision(true);
	// OnComponentHit 델리게이트에 OnHit 함수 바인딩
	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	
	// BulletMesh의 Physics 사용 설정
	//BulletMesh->SetSimulatePhysics(true);
	
	// Overlap 이벤트를 실행하기 위해 활성화
	//BulletMesh->SetGenerateOverlapEvents(true);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	// ProjectileMovementComponent는 BulletMesh를 기준으로 움직임을 계산하고 다른 오브젝트와 충돌하는지를 감지하며 물리적 반응을 처리
	ProjectileMovementComponent->UpdatedComponent = BulletMesh;
	// Bullet 발사 속도
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 1. OtherActor가 nullptr인지 확인
	// 2. OtherActor가 Owner(Weapon)인지 확인
	// 3. OtherActor가 Instigator(Weapon을 소유한 캐릭터)인지 확인
	if (OtherActor == nullptr || OtherActor == GetOwner() || GetInstigator() && OtherActor == GetInstigator())
	{
		Destroy();
		return;
	}
	
	if (OtherActor && GetInstigatorController())
	{
		UGameplayStatics::ApplyDamage(OtherActor, 20.0f, GetInstigatorController(), this, UDamageType::StaticClass());

		// WeaponBase의 BulletHitEnemy 함수가 바인딩 되어 있으면 해당 함수 실행
		OnBulletHitEnemyDelegate.ExecuteIfBound(OtherActor);
	}

	if (OtherComp)
	{
		// 발사 방향
		FVector BulletDirection = GetVelocity();
		float ForceStrength = 5.0f;

		// OtherComp가 물리 시뮬레이션 중인지 확인
		if (OtherComp->IsSimulatingPhysics())
		{
			// 힘을 가하기
			OtherComp->AddImpulse(BulletDirection * ForceStrength, NAME_None, false);
		}

		// DrawDebugLine(
		// 	GetWorld(),
		// 	BulletMesh->GetCenterOfMass(),
		// 	BulletDirection * 200.0f,
		// 	FColor::Red,
		// 	true,       
		// 	5.0f,       
		// 	0,          
		// 	5.0f        
		// );
		// DrawDebugPoint( 
		// 	GetWorld(),
		// 	GetActorLocation(),
		// 	15.0f,
		// 	FColor::Blue,
		// 	true, 5.0f
		// );
	}

	// Hit 지점에 HitEffect 생성
	if (HitEffect && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, FRotator::ZeroRotator, FVector(0.2f));
	}
	
	// 3초 후 Destroy
	//InitialLifeSpan = 3.0f;

	Destroy();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

