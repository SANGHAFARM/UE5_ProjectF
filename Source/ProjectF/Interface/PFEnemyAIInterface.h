// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PFEnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPFEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAIAttackFinishedDelegate);
/**
 * 
 */
class PROJECTF_API IPFEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAIAttackDelegate(const FAIAttackFinishedDelegate& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;
};
