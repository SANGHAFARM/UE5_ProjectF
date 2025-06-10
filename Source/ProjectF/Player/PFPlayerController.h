// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PFPlayerController.generated.h"

class UPFHUDWidget;
/**
 * 
 */
UCLASS()
class PROJECTF_API APFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APFPlayerController();

	virtual void BeginPlay() override;

	FORCEINLINE UPFHUDWidget* GetPFHUDWidget() const { return PFHUDWidget; }
	
	// HUD Section
protected:
	// 생성할 위젯의 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<UPFHUDWidget> PFHUDWidgetClass;

	// 생성한 위젯을 객체 정보를 저장할 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<UPFHUDWidget> PFHUDWidget;
};
