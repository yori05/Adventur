// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventurGameMode.h"
#include "Character/AdventurCharacter.h"
#include "Character/AdvancedCharacter.h"
#include "Controller/AdvancedPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AAdventurGameMode::AAdventurGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/AdvancedCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Controller/AdvancedPlayerController_BP"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

}
