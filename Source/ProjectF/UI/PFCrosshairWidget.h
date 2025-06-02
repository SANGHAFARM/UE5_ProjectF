// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PFCrosshairWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPFCrosshairWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	// UMG가 초기화될 때 호출되는 함수
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> Top;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> Left;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> Right;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> Bottom;
};
