// Fill out your copyright notice in the Description page of Project Settings.


#include "PFCrosshairWidget.h"
#include "Components/TextBlock.h"

UPFCrosshairWidget::UPFCrosshairWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPFCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Top = Cast<UTextBlock>(GetWidgetFromName("Crosshair_Top"));
	Left = Cast<UTextBlock>(GetWidgetFromName("Crosshair_Left"));
	Right = Cast<UTextBlock>(GetWidgetFromName("Crosshair_Right"));
	Bottom= Cast<UTextBlock>(GetWidgetFromName("Crosshair_Bottom"));
}
