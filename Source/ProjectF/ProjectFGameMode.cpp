// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectFGameMode.h"
#include "ProjectFCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectFGameMode::AProjectFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
