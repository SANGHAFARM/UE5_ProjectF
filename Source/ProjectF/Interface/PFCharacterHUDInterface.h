// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PFCharacterHUDInterface.generated.h"



class UPFHUDWidget;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPFCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTF_API IPFCharacterHUDInterface
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 캐릭터에서 HUD 위젯 설정
	virtual void SetupHUDWidget(UPFHUDWidget* InHUDWidget) = 0;

};
