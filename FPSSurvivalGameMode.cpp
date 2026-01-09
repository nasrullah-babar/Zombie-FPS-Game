// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSSurvivalGameMode.h"
#include "FPSSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSSurvivalGameMode::AFPSSurvivalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
