// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PFHPWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPFHPWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateHPProgressBar(float CurrentHP, float MaxHP);
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<UProgressBar> HPProgressBar;
};
