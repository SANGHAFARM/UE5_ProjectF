// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFAmmoWidget.h"
#include "Blueprint/UserWidget.h"
#include "PFHUDWidget.generated.h"

class UPFCrosshairWidget;
class UPFAmmoWidget;
/**
 * 
 */
UCLASS()
class PROJECTF_API UPFHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPFHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void HideCrosshair(bool bSetHide);

	void UpdateAmmo(uint32 CurrentAmmo, uint32 MaxAmmo);
	
protected:
	// UMG가 초기화될 때 호출되는 함수
	virtual void NativeConstruct() override;
	
private:
	// Crosshair 위젯
	UPROPERTY()
	TObjectPtr<UPFCrosshairWidget> CrosshairWidget;
	
	// Ammo 위젯
	UPROPERTY()
	TObjectPtr<UPFAmmoWidget> AmmoWidget;
};
