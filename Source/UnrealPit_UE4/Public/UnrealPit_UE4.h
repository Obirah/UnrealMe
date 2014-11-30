// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#ifndef __UNREALPIT_UE4_H__
#define __UNREALPIT_UE4_H__

#include "Engine.h"

#define COLLISION_PROJECTILE	ECC_GameTraceChannel1

//General Log
DECLARE_LOG_CATEGORY_EXTERN(UnrealPit, Log, All);
//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(UnrealPitInit, Log, All);
//Logging for the VRPN component
DECLARE_LOG_CATEGORY_EXTERN(UnrealPit_VRPN, Log, All);
//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(UnrealPitCriticalErrors, Log, All);

#endif
