// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PFHPWidget.h"

#include "Components/ProgressBar.h"

UPFHPWidget::UPFHPWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPFHPWidget::UpdateHPProgressBar(float CurrentHP, float MaxHP)
{
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UPFHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Pb_HPBar")));
	ensure(HPProgressBar);
}
