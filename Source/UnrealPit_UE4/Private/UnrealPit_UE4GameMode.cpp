// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UnrealPit_UE4.h"
#include "UnrealPit_UE4GameMode.h"
#include "UnrealPit_UE4HUD.h"
#include "UnrealPit_UE4Character.h"

AUnrealPit_UE4GameMode::AUnrealPit_UE4GameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUnrealPit_UE4HUD::StaticClass();
}
