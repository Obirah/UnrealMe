// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UnrealPit_UE4.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, UnrealPit_UE4, "UnrealPit_UE4" );

//General Log
DEFINE_LOG_CATEGORY(UnrealPit);
//Logging during game startup
DEFINE_LOG_CATEGORY(UnrealPitInit);
//Logging for the VRPN component
DEFINE_LOG_CATEGORY(UnrealPit_VRPN);
//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(UnrealPitCriticalErrors);
 