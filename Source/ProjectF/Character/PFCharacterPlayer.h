// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PFCharacterBase.h"
#include "PFCharacterPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class PROJECTF_API APFCharacterPlayer : public APFCharacterBase
{
	GENERATED_BODY()

public:
	APFCharacterPlayer();
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 조작
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 카메라
protected:
	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

	// 입력
protected:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	
};
