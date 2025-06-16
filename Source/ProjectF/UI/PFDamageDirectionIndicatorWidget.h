// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PFDamageDirectionIndicatorWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFDamageDirectionIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPFDamageDirectionIndicatorWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UImage> IndicatorImage;
};
