// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PFAmmoWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPFAmmoWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// UMG가 초기화될 때 호출되는 함수
	virtual void NativeConstruct() override;

protected:
	// 현재 탄약
	UPROPERTY()
	TObjectPtr<UTextBlock> CurrentAmmo;

	// 최대 탄약
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxAmmo;
};
