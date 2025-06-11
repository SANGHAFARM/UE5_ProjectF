// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PFEnemyAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = "CanAttack";
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	IPFEnemyAIInterface* AIPawn = Cast<IPFEnemyAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr)
	{
		return false;
	}

	// Target과 자신의 거리 체크
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}
