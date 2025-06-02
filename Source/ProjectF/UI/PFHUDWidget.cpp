// Fill out your copyright notice in the Description page of Project Settings.


#include "PFHUDWidget.h"

#include "PFAmmoWidget.h"
#include "PFCrosshairWidget.h"

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

void UPFHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CrosshairWidget = Cast<UPFCrosshairWidget>(GetWidgetFromName("WBP_Crosshair"));
	ensure(CrosshairWidget);

	AmmoWidget = Cast<UPFAmmoWidget>(GetWidgetFromName("WBP_Ammo"));
	ensure(AmmoWidget);
}
