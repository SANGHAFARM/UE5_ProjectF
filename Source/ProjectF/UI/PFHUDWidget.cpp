// Fill out your copyright notice in the Description page of Project Settings.


#include "PFHUDWidget.h"

#include "PFAmmoWidget.h"
#include "PFCrosshairWidget.h"
#include "ProjectF/Interface/PFCharacterHUDInterface.h"

UPFHUDWidget::UPFHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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
	// 델리게이트로 호출된 이벤트 함수
	// AmmoWidget의 Text를 매개 변수로 받은 값으로 설정
	AmmoWidget->SetCurrentAmmoText(CurrentAmmo);
	AmmoWidget->SetMaxAmmoText(MaxAmmo);
}

void UPFHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이름으로 검색해 Crosshair 위젯 설정
	CrosshairWidget = Cast<UPFCrosshairWidget>(GetWidgetFromName("WBP_Crosshair"));
	ensure(CrosshairWidget);

	// 이름으로 검색해 Ammo 위젯 설정
	AmmoWidget = Cast<UPFAmmoWidget>(GetWidgetFromName("WBP_Ammo"));
	ensure(AmmoWidget);
	
	IPFCharacterHUDInterface* PawnHUD = Cast<IPFCharacterHUDInterface>(GetOwningPlayerPawn());
	if (PawnHUD)
	{
		PawnHUD->SetupHUDWidget(this);
	}
}
