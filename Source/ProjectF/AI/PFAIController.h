// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PFAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTF_API APFAIController : public AAIController
{
	GENERATED_BODY()

public:
	APFAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	// Target Key의 Value 값을 Player로 설정 
	void SetTarget();

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
};
