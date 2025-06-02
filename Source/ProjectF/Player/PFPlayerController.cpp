// Fill out your copyright notice in the Description page of Project Settings.


#include "PFPlayerController.h"

#include "../UI/PFHUDWidget.h"

APFPlayerController::APFPlayerController()
{
	// 위젯 블루프린트 에셋에서 클래스 정보 로드
	static ConstructorHelpers::FClassFinder<UPFHUDWidget> PFHUDWidgetRef(TEXT("/Game/ProjectF/Blueprints/UI/WBP_PlayerUI.WBP_PlayerUI_C"));
	if (PFHUDWidgetRef.Class)
	{
		PFHUDWidgetClass = PFHUDWidgetRef.Class;
	}
}

void APFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 생성
	PFHUDWidget = CreateWidget<UPFHUDWidget>(this, PFHUDWidgetClass);
	if (PFHUDWidget)
	{
		PFHUDWidget->AddToViewport();
	}
}
