// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PFDamageDirectionIndicatorWidget.h"

#include "Components/Image.h"

UPFDamageDirectionIndicatorWidget::UPFDamageDirectionIndicatorWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPFDamageDirectionIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IndicatorImage = Cast<UImage>(GetWidgetFromName(TEXT("IndicatorImg")));
	ensure(IndicatorImage);
}
