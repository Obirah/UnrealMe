// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UnrealMe.h"
#include "UnrealMeGameMode.h"
#include "UnrealMeHUD.h"
#include "UnrealMeCharacter.h"

AUnrealMeGameMode::AUnrealMeGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/AnimStarterPack/Character/UnrealMeKinectCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter")); 
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUnrealMeHUD::StaticClass();
}
