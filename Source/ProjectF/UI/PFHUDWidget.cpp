// Fill out your copyright notice in the Description page of Project Settings.


#include "PFHUDWidget.h"

#include "PFAmmoWidget.h"
#include "PFCrosshairWidget.h"
#include "PFDamageDirectionIndicatorWidget.h"
#include "PFHPWidget.h"
#include "PFSurvivalTimerWidget.h"
#include "Game/PFGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectF/Interface/PFCharacterHUDInterface.h"

UPFHUDWidget::UPFHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPFHUDWidget::SetHideHUDWidget()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPFHUDWidget::HideCrosshair(bool bSetHide)
{
	if (bSetHide)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPFHUDWidget::UpdateAmmo(uint32 CurrentAmmo, uint32 MaxAmmo)
{
	if (AmmoWidget)
	{
		// 델리게이트로 호출된 이벤트 함수
		// AmmoWidget의 Text를 매개 변수로 받은 값으로 설정
		AmmoWidget->SetCurrentAmmoText(CurrentAmmo);
		AmmoWidget->SetMaxAmmoText(MaxAmmo);
	}
}

void UPFHUDWidget::ChangeCrosshairColor(FLinearColor InColor)
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetColorAndOpacity(InColor);
	}
}

void UPFHUDWidget::UpdateHP(float NewCurrentHP, float MaxHP)
{
	if (HPWidget)
	{
		HPWidget->UpdateHPProgressBar(NewCurrentHP, MaxHP);
	}
}

void UPFHUDWidget::UpdateDamageDirectionIndicator(float NewAngle)
{
	if (DamageDirectionIndicatorWidget)
	{
		DamageDirectionIndicatorWidget->SetRenderTransformAngle(NewAngle);
	}
}

void UPFHUDWidget::UpdateSurvivalTimer(int32 NewTime)
{
	if (SurvivalTimerWidget)
	{
		SurvivalTimerWidget->SetRemainTimeText(NewTime);
	}
}

void UPFHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이름으로 검색해 Crosshair 위젯 설정
	CrosshairWidget = Cast<UPFCrosshairWidget>(GetWidgetFromName(TEXT("WBP_Crosshair")));
	ensure(CrosshairWidget);

	// 이름으로 검색해 Ammo 위젯 설정
	AmmoWidget = Cast<UPFAmmoWidget>(GetWidgetFromName(TEXT("WBP_Ammo")));
	ensure(AmmoWidget);

	HPWidget = Cast<UPFHPWidget>(GetWidgetFromName(TEXT("WBP_HPBar")));
	ensure(HPWidget);

	DamageDirectionIndicatorWidget = Cast<UPFDamageDirectionIndicatorWidget>(GetWidgetFromName(TEXT("WBP_DamageDirectionIndicator")));
	ensure(DamageDirectionIndicatorWidget);
	
	IPFCharacterHUDInterface* PawnHUD = Cast<IPFCharacterHUDInterface>(GetOwningPlayerPawn());
	if (PawnHUD)
	{
		PawnHUD->SetupHUDWidget(this);
	}
	
	SurvivalTimerWidget = Cast<UPFSurvivalTimerWidget>(GetWidgetFromName(TEXT("WBP_SurvivalTimer")));
	ensure(SurvivalTimerWidget);

	APFGameMode* GameMode = Cast<APFGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		// PFGameMode의 델리게이트에 HUD의 남은 시간 텍스트를 업데이트하는 함수 바인딩
		GameMode->OnUpdateSurvivalRemainTime.BindUObject(this, &UPFHUDWidget::UpdateSurvivalTimer);
		SurvivalTimerWidget->SetRemainTimeText(GameMode->GetRemainTime());
	}
}
