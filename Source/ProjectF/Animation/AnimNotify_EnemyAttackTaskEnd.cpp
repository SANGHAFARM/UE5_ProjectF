// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_EnemyAttackTaskEnd.h"

#include "Interface/PFEnemyAnimationInterface.h"

void UAnimNotify_EnemyAttackTaskEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	IPFEnemyAnimationInterface* EnemyPawn = Cast<IPFEnemyAnimationInterface>(MeshComp->GetOwner());
	if (EnemyPawn)
	{
		EnemyPawn->OnAttackTaskEnd();
	}
}
