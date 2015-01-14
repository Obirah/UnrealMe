// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#ifndef __UNREALME_H__
#define __UNREALME_H__

#include "Engine.h"

#define COLLISION_PROJECTILE	ECC_GameTraceChannel1

//General Log
DECLARE_LOG_CATEGORY_EXTERN(UnrealMe, Log, All);
//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(UnrealMeInit, Log, All);
//Logging for the VRPN component
DECLARE_LOG_CATEGORY_EXTERN(UnrealMe_VRPN, Log, All);
//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(UnrealMeCriticalErrors, Log, All);

#endif
