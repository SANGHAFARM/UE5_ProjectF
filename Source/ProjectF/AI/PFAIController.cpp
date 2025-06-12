// Fill out your copyright notice in the Description page of Project Settings.


#include "PFAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

APFAIController::APFAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/ProjectF/AI/BB_PFEnemy.BB_PFEnemy"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/ProjectF/AI/BT_PFEnemy.BT_PFEnemy"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void APFAIController::RunAI()
{
	// UBlackboardComponent는 UseBlackboard 함수 실행 시 생성
	// UseBlackboard 함수 실행 전까지는 포인터가 NULL을 가리킴
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APFAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void APFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// OnPossess 함수 실행 후 AI 로직 실행
	RunAI();
}

void APFAIController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 Target을 Player로 설정
	SetTarget();
}

void APFAIController::SetTarget()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	UWorld* World = GetPawn()->GetWorld();
	if (BlackboardPtr && World)
	{
		// GetPlayerCharacter 함수로 월드에 있는 플레이어를 불러와서 Target에 저장
		ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (PlayerCharacter)
		{
			BlackboardPtr->SetValueAsObject(TEXT("Target"), PlayerCharacter);
		}
	}
}
