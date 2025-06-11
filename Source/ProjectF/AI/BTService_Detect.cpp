// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// BlackboardComponent에 Target 값이 설정되었으면 return
	if (bValueHasSet)
	{
		return;
	}
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp && OwnerComp.GetWorld())
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0));
		if (PlayerCharacter)
		{
			BlackboardComp->SetValueAsObject(TEXT("Target"), PlayerCharacter);
			bValueHasSet = true;
		}
	}
}
