// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_EnemyHandAttackStart.h"

#include "Interface/PFEnemyAnimationInterface.h"

void UAnimNotify_EnemyHandAttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAnimInstance* MeshAnimInstance = MeshComp->GetAnimInstance();
	if (MeshAnimInstance)
	{
		FName SectionName = MeshAnimInstance->Montage_GetCurrentSection();

		IPFEnemyAnimationInterface* EnemyPawn = Cast<IPFEnemyAnimationInterface>(MeshComp->GetOwner());
		if (EnemyPawn)
		{
			EnemyPawn->EnableAttackCollision(SectionName);
		}
	}
}
