// Copyright Epic Games, Inc. All Rights Reserved.

#include "Fighter_Combat_Test1GameMode.h"
#include "Fighter_Combat_Test1Character.h"
#include "UObject/ConstructorHelpers.h"

AFighter_Combat_Test1GameMode::AFighter_Combat_Test1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
