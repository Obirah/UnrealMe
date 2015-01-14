// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UnrealMe.h"


IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, UnrealMe, "UnrealMe" );

//General Log
DEFINE_LOG_CATEGORY(UnrealMe);
//Logging during game startup
DEFINE_LOG_CATEGORY(UnrealMeInit);
//Logging for the VRPN component
DEFINE_LOG_CATEGORY(UnrealMe_VRPN);
//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(UnrealMeCriticalErrors);