// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPit_UE4.h"
#include "UnrealPit_UE4VRPN.h"

typedef void(*_initializeTracker)(); //our typedef is must be the same type of our dll function.
typedef void(*_callTrackerMainloop)();
typedef FVector* (*_getKinectBonePosition)(int boneId);
typedef QuatVector* (*_getQuaternion)(int boneId);

static void *DLLHandle;

UUnrealPit_UE4VRPN::UUnrealPit_UE4VRPN(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	UE_LOG(UnrealPitInit, Log, TEXT("Construction of VRPN"));

	FString filePath = FPaths::Combine(*FPaths::GameDir(), TEXT("Binaries/Win64/"), TEXT("UnrealVRPNx64.dll"));

	if (FPaths::FileExists(filePath))
	{
		UE_LOG(UnrealPitInit, Log, TEXT("Filepath exists, creating DLLHandle for file: %s"), *filePath);
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath); // get dll

		if (DLLHandle != NULL)
		{
			initializeTracker();
		}
		else
		{
			UE_LOG(UnrealPitCriticalErrors, Error, TEXT("DLL Handle is NULL during initialization of VRPN client."));
		}
	}
}

/* Doesn't need to check DLLHandle for NULL because this check happens when the function is called in the constructor. */
void UUnrealPit_UE4VRPN::initializeTracker()
{
	UE_LOG(UnrealPitInit, Log, TEXT("Assigning initialize function from DLL."));
	_initializeTracker DLLgetInitializeTracker = NULL; // the container for our dll function
	FString procName = "initializeTracker"; // the exact name of our dll function to recover
	DLLgetInitializeTracker = (_initializeTracker)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need

	if (DLLgetInitializeTracker != NULL)
	{
		UE_LOG(UnrealPitInit, Log, TEXT("Calling initialize DLL function."));
		DLLgetInitializeTracker();
		callTrackerMainloop();
	}
	else
	{
		UE_LOG(UnrealPitCriticalErrors, Error, TEXT("Handle for initialize function is NULL."));
	}
}

/* Doesn't need to check if DLLHadnle is NULL, because it's called from within "initializeTracker" where the handle can't be NULL. */
void UUnrealPit_UE4VRPN::callTrackerMainloop()
{
	UE_LOG(UnrealPitInit, Log, TEXT("Assigning tracker mainloop function from DLL."));
	_callTrackerMainloop DLLcallTrackerMainloop = NULL; // the container for our dll function
	FString procName = "callTrackerMainloop"; // the exact name of our dll function to recover
	DLLcallTrackerMainloop = (_callTrackerMainloop)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need

	if (DLLcallTrackerMainloop != NULL)
	{
		UE_LOG(UnrealPitInit, Log, TEXT("Calling tracker mainloop DLL function."));
		DLLcallTrackerMainloop();
	}
	else
	{
		UE_LOG(UnrealPitCriticalErrors, Error, TEXT("Handle for mainloop function is NULL."));
	}
}

FVector* UUnrealPit_UE4VRPN::getKinectBonePosition(int aBoneId)
{
	FVector* tBack = NULL;

	if (DLLHandle != NULL)
	{
		_getKinectBonePosition DLLgetKinectBonePosition = NULL; // the container for our dll function
		FString procName = "getKinectBonePosition"; // the exact name of our dll function to recover
		DLLgetKinectBonePosition = (_getKinectBonePosition)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need

		if (DLLgetKinectBonePosition != NULL)
		{
			tBack = DLLgetKinectBonePosition(aBoneId);
		}
		else
		{
			UE_LOG(UnrealPitCriticalErrors, Error, TEXT("Handle for getPosition function is NULL."));
		}
	}
	else
	{
		UE_LOG(UnrealPitCriticalErrors, Error, TEXT("DLLHandle is NULL. (called from getPosition function.)"));
	}

	return tBack;
}

QuatVector* UUnrealPit_UE4VRPN::getQuaternion(int aBoneId)
{
	QuatVector* tBack = NULL;

	if (DLLHandle != NULL)
	{
		_getQuaternion DLLgetQuaternion = NULL; // the container for our dll function
		FString procName = "getQuaternion"; // the exact name of our dll function to recover
		DLLgetQuaternion = (_getQuaternion)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need
		if (DLLgetQuaternion != NULL)
		{
			QuatVector* tBack = DLLgetQuaternion(aBoneId);
		}
		else
		{
			UE_LOG(UnrealPitCriticalErrors, Error, TEXT("Handle for getQuaternion function is NULL."));
		}
	}
	else
	{
		UE_LOG(UnrealPitCriticalErrors, Error, TEXT("DLLHandle is NULL. (Called from getQuaternion function.)"));
	}

	return tBack;
}