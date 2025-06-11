// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
