// Copyright Epic Games, Inc. All Rights Reserved.

#include "MasterProject21GameMode.h"
#include "MasterProject21Character.h"
#include "UObject/ConstructorHelpers.h"

AMasterProject21GameMode::AMasterProject21GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
