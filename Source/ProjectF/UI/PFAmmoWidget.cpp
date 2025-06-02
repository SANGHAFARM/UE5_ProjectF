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

	CurrentAmmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtCurrentAmmo")));
	ensure(CurrentAmmo);
	
	MaxAmmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxAmmo")));
	ensure(MaxAmmo);
	
	// 하고 싶은 것 : 캐릭터에 내 정보(위젯)을 전달
	// 강참조를 피하기 위해 인터페이스를 통해 우회 전달 (느슨한 결합(참조))
	// IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	// if (CharacterWidget)
	// {
	// 	// 인터페이스를 통해서 캐릭터에 내 정보(위젯) 전달
	// 	CharacterWidget->SetupCharacterWidget(this);
	// }
}
