// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PFSurvivalTimerWidget.h"

#include "Components/TextBlock.h"

UPFSurvivalTimerWidget::UPFSurvivalTimerWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPFSurvivalTimerWidget::SetRemainTimeText(uint32 NewRemainTime)
{
	if (RemainTimeText)
	{
		RemainTimeText->SetText(FText::AsNumber(NewRemainTime));
	}
}

void UPFSurvivalTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RemainTimeText = Cast<UTextBlock>(GetWidgetFromName("RemainTimeText"));
	ensure(RemainTimeText);
}
