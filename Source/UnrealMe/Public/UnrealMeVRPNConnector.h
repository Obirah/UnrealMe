// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "AllowWindowsPlatformTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <vrpn_Tracker.h>
#include "HideWindowsPlatformTypes.h"
#include "UnrealMeVRPNBone.h"
#include "UnrealMeCoordinateHelper.h"
#include "UnrealMeVRPNConnector.generated.h"

/**
 * 
 */
UCLASS()
class UNREALME_API UUnrealMeVRPNConnector : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static void initializeConnection(TArray<FString> aTrackerNames, FString aServerAddress);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static void destroyConnection();
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static void callMainloop();
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static FVector getBonePosition(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static FRotator getBoneRotation(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static FQuat getBoneQuaternion(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static float getBoneAcceleration(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static float getBoneVelocity(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static FRotator getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static bool isConnected();
};
