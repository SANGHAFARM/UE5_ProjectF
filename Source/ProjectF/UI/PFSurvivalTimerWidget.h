// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PFSurvivalTimerWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFSurvivalTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPFSurvivalTimerWidget(const FObjectInitializer& ObjectInitializer);

	void SetRemainTimeText(uint32 NewRemainTime);
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> RemainTimeText;
};
