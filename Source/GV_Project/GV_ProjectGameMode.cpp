// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GV_ProjectGameMode.h"
#include "GV_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGV_ProjectGameMode::AGV_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
