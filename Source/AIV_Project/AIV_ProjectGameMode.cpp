// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIV_ProjectGameMode.h"
#include "AIV_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAIV_ProjectGameMode::AAIV_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
