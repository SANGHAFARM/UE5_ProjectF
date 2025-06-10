// Fill out your copyright notice in the Description page of Project Settings.


#include "PFAmmoWidget.h"

#include "Components/TextBlock.h"

UPFAmmoWidget::UPFAmmoWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPFAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtCurrentAmmo")));
	ensure(CurrentAmmoText);
	
	MaxAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxAmmo")));
	ensure(MaxAmmoText);
}
