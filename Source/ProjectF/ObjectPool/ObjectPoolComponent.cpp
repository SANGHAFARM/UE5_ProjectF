// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"

#include "Interface/ObjectPoolInterface.h"

// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	
}

AActor* UObjectPoolComponent::SpawnPooledObject()
{
	// PooledObjectsArray를 순회하면서 비활성화된 액터 찾기
	for (AActor* ArrayActor : PooledObjectsArray)
	{
		IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(ArrayActor);
		// 비활성화된 액터를 찾았으면 액터의 위치와 회전을 초기화하고 수명을 설정하고 액터를 활성화
		if (PoolableActor && PoolableActor->IsPoolActive_Implementation() == false)
		{
			ArrayActor->TeleportTo(FVector(0), FRotator(0));
			PoolableActor->SetPooledObjectLifeSpan_Implementation(PooledObjectLifespan);
			PoolableActor->PooledObjectSetActive_Implementation(true);
			// 현재 활성화 중인 액터를 추적하기 위해 SpawnedPoolIndexes에 현재 액터의 인덱스 추가
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex_Implementation());

			return ArrayActor;
		}
	}

	// 비활성화된 액터가 없을 경우
	if (SpawnedPoolIndexes.Num() > 0)
	{
		// 가장 먼저(오래된) 활성화한 액터를 회수 대상으로 선정
		int PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledObjectIndex);
		
		if (PooledObjectsArray.IsValidIndex(PooledObjectIndex))
		{
			AActor* PoolActor = PooledObjectsArray[PooledObjectIndex];
			
			IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(PoolActor);
			// 회수하려는 액터를 비활성화한 후 다시 활성화
			if (PoolableActor)
			{
				PoolActor->TeleportTo(FVector(0), FRotator(0));
				
				PoolableActor->PooledObjectSetActive_Implementation(false);
				PoolableActor->SetPooledObjectLifeSpan_Implementation(PooledObjectLifespan);
				PoolableActor->PooledObjectSetActive_Implementation(true);
				SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex_Implementation());

				return PoolActor;
			}
		}
	}

	return nullptr;
}

void UObjectPoolComponent::OnPooledObjectDespawn(AActor* PoolActor)
{
	IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(PoolActor);
	if (PoolableActor)
	{
		SpawnedPoolIndexes.Remove(PoolableActor->GetPoolIndex_Implementation());
	}
}

int UObjectPoolComponent::GetPoolSize()
{
	if (PooledObjectsArray.Num() <= 0)
	{
		return 0;
	}

	return PoolSize;
}

AActor* UObjectPoolComponent::GetIndexedPooledObject(int Index)
{
	if (PooledObjectsArray.IsValidIndex(Index) == false)
	{
		return nullptr;
	}

	return PooledObjectsArray[Index];
}

// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectSubclass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// PooledObjectSubclass에 ObjectPoolInterface가 구현되어 있는지 확인
			if (PooledObjectSubclass->ImplementsInterface(UObjectPoolInterface::StaticClass()) == false)
			{
				return;
			}

			// 풀 객체들을 제거하고 빈 배열로 초기화
			PooledObjectsArray.Empty();
			// PoolSize만큼 액터 생성
			for (int i = 0; i < PoolSize; i++)
			{
				// 액터 스폰
				AActor* PoolActor = World->SpawnActor<AActor>(PooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
				if (PoolActor)
				{
					// 스폰된 액터에 ObjectPoolInterface가 구현되어 있는지 확인
					IObjectPoolInterface* PoolableActor = Cast<IObjectPoolInterface>(PoolActor);
					if (PoolableActor)
					{
						// 스폰 직후에 액터를 비활성화하고, 오브젝트 풀에서의 인덱스 설정
						PoolableActor->PooledObjectSetActive_Implementation(false);
						PoolableActor->SetPoolIndex_Implementation(i);
						// OnPooledObjectDespawn 델리게이트에 UObjectPoolComponent::OnPooledObjectDespawn 함수 바인딩
						PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPoolComponent::OnPooledObjectDespawn);
						// 오브젝트 풀 배열에 등록
						PooledObjectsArray.Add(PoolActor);
					}
					else
					{
						PoolActor->Destroy();
					}
				}
			}
		}
	}
}