// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeVRPN.h"

typedef void(*_init)(); //our typedef is must be the same type of our dll function.
typedef void(*_callTrackerMainloop)();
typedef FVector (*_getBonePosition)(int boneId);
typedef float(*_getX)(int boneId);
typedef FQuatVector* (*_getQuaternion)(int boneId);

static void *DLLHandle;

UUnrealMeVRPN::UUnrealMeVRPN(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	UE_LOG(UnrealMeInit, Log, TEXT("Construction of VRPN"));

	FString filePath = FPaths::Combine(*FPaths::GameDir(), TEXT("Binaries/Win64/"), TEXT("UnrealVRPNx64.dll"));

	if (FPaths::FileExists(filePath))
	{
		UE_LOG(UnrealMeInit, Log, TEXT("Filepath exists, creating DLLHandle for file: %s"), *filePath);
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath); // get dll

		if (DLLHandle != NULL)
		{
			initializeTracker();
		}
		else
		{
			UE_LOG(UnrealMeCriticalErrors, Error, TEXT("DLL Handle is NULL during initialization of VRPN client."));
		}
	}
}

/* Doesn't need to check DLLHandle for NULL because this check happens when the function is called in the constructor. */
void UUnrealMeVRPN::initializeTracker()
{
	UE_LOG(UnrealMeInit, Log, TEXT("Construction of VRPN"));

	FString filePath = FPaths::Combine(*FPaths::GameDir(), TEXT("Binaries/Win64/"), TEXT("UnrealVRPNx64.dll"));

	if (FPaths::FileExists(filePath))
	{
		UE_LOG(UnrealMeInit, Log, TEXT("Filepath exists, creating DLLHandle for file: %s"), *filePath);
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath); // get dll

		if (DLLHandle != NULL)
		{
			UE_LOG(UnrealMeInit, Log, TEXT("Assigning initialize function from DLL."));
			_init DLLgetInitializeTracker = NULL; // the container for our dll function
			FString procName = "initializeTracker"; // the exact name of our dll function to recover
			DLLgetInitializeTracker = (_init)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need

			if (DLLgetInitializeTracker != NULL)
			{
				UE_LOG(UnrealMeInit, Log, TEXT("Calling initialize DLL function."));
				DLLgetInitializeTracker();
			}
			else
			{
				UE_LOG(UnrealMeCriticalErrors, Error, TEXT("Handle for initialize function is NULL."));
			}
		}
		else
		{
			UE_LOG(UnrealMeCriticalErrors, Error, TEXT("DLL Handle is NULL during initialization of VRPN client."));
		}
	}
}

void UUnrealMeVRPN::callTrackerMainloop()
{
	if (DLLHandle != NULL)
	{
		UE_LOG(UnrealMeInit, Log, TEXT("Assigning tracker mainloop function from DLL."));
		_callTrackerMainloop DLLcallTrackerMainloop = NULL; // the container for our dll function
		FString procName = "callTrackerMainloop"; // the exact name of our dll function to recover
		DLLcallTrackerMainloop = (_callTrackerMainloop)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need

		if (DLLcallTrackerMainloop != NULL)
		{
			UE_LOG(UnrealMeInit, Log, TEXT("Calling tracker mainloop DLL function."));
			DLLcallTrackerMainloop();
		}
		else
		{
			UE_LOG(UnrealMeCriticalErrors, Error, TEXT("Handle for mainloop function is NULL."));
		}
	}
}

FVector UUnrealMeVRPN::getBonePosition(int32 aBoneId)
{
	FVector tBack = FVector(-1, -1, -1);

	if (DLLHandle != NULL)
	{
		_getBonePosition DLLgetBonePosition = NULL; // the container for our dll function
		FString procName = "getBonePosition"; // the exact name of our dll function to recover
		DLLgetBonePosition = (_getBonePosition)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need

		if (DLLgetBonePosition != NULL)
		{
			tBack = DLLgetBonePosition(aBoneId);
		}
		else
		{
			UE_LOG(UnrealMeCriticalErrors, Error, TEXT("Handle for getPosition function is NULL."));
		}
	}
	else
	{
		UE_LOG(UnrealMeCriticalErrors, Error, TEXT("DLLHandle is NULL. (called from getPosition function.)"));
	}

	return tBack;
}

float UUnrealMeVRPN::getX(int32 aBoneId)
{
	float tBack = 0.1;

	if (DLLHandle != NULL)
	{
		_getX DLLgetX = NULL; // the container for our dll function
		FString procName = "getX"; // the exact name of our dll function to recover
		DLLgetX = (_getX)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need

		if (DLLgetX != NULL)
		{
			tBack = DLLgetX(aBoneId);
		}
		else
		{
			UE_LOG(UnrealMeCriticalErrors, Error, TEXT("Handle for getPosition function is NULL."));
		}
	}
	else
	{
		UE_LOG(UnrealMeCriticalErrors, Error, TEXT("DLLHandle is NULL. (called from getX function.)"));
	}

	return tBack;
}

FQuatVector UUnrealMeVRPN::getQuaternion(int32 aBoneId)
{
	FQuatVector* tBack = NULL;

	if (DLLHandle != NULL)
	{
		_getQuaternion DLLgetQuaternion = NULL; // the container for our dll function
		FString procName = "getQuaternion"; // the exact name of our dll function to recover
		DLLgetQuaternion = (_getQuaternion)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get the dll function need
		if (DLLgetQuaternion != NULL)
		{
			FQuatVector* tBack = DLLgetQuaternion(aBoneId);
		}
		else
		{
			UE_LOG(UnrealMeCriticalErrors, Error, TEXT("Handle for getQuaternion function is NULL."));
		}
	}
	else
	{
		UE_LOG(UnrealMeCriticalErrors, Error, TEXT("DLLHandle is NULL. (Called from getQuaternion function.)"));
	}

	return *tBack;
}