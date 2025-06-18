// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFAmmoWidget.h"
#include "Blueprint/UserWidget.h"
#include "PFHUDWidget.generated.h"

class UPFSurvivalTimerWidget;
class UPFDamageDirectionIndicatorWidget;
class UPFHPWidget;
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

	void SetHideHUDWidget();

public:
	// Crosshair 표시 여부 함수
	void HideCrosshair(bool bSetHide);

	// AmmoWidget의 Ammo 텍스트 업데이트 함수
	void UpdateAmmo(uint32 CurrentAmmo, uint32 MaxAmmo);

	// CrosshairWidget의 색상을 설정하는 함수
	void ChangeCrosshairColor(FLinearColor InColor);

	// HPWidget의 프로그레스바를 업데이트하는 함수
	void UpdateHP(float NewCurrentHP, float MaxHP);

	// DamageDirectionIndicator의 이미지 각도를 업데이트하는 함수
	void UpdateDamageDirectionIndicator(float NewAngle);

	// 블루프린트에서 구현할 함수 선언
	// Hitmarker 애니메이션을 재생하는 함수
	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitmarker(bool bIsDead);

	// Indicator 애니메이션을 재생하는 함수
	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageDirectionIndicatorAnimation();

	// SurvivalTimer 텍스트를 업데이트하는 함수
	void UpdateSurvivalTimer(int32 NewTime);
	
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

	// HP 위젯
	UPROPERTY()
	TObjectPtr<UPFHPWidget> HPWidget;

	// Indicator 위젯
	UPROPERTY()
	TObjectPtr<UPFDamageDirectionIndicatorWidget> DamageDirectionIndicatorWidget;

	// SurvivalTimer 위젯
	UPROPERTY()
	TObjectPtr<UPFSurvivalTimerWidget> SurvivalTimerWidget;
};
