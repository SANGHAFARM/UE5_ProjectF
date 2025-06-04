// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

public:
	FORCEINLINE void SetCurrentAmmoText(uint32 InCurrentAmmo) const { CurrentAmmoText->SetText(FText::AsNumber(InCurrentAmmo)); }
	FORCEINLINE void SetMaxAmmoText(uint32 InMaxAmmo) const { MaxAmmoText->SetText(FText::AsNumber(InMaxAmmo)); }
	
protected:
	// UMG가 초기화될 때 호출되는 함수
	virtual void NativeConstruct() override;

protected:
	// 현재 탄약
	UPROPERTY()
	TObjectPtr<UTextBlock> CurrentAmmoText;

	// 최대 탄약
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxAmmoText;
};
