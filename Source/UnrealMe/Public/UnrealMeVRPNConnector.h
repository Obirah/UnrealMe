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
private:
	static UUnrealMeVRPNConnector *iSingleton;
	TArray<vrpn_Tracker_Remote*> iTrackerRemotes;
	vrpn_Connection	*iConnection;

	bool iConnected;

	int32 iPosArrLen;
	int32 iRotArrLen;
	int32 iConnectionPort;
public:
	UUnrealMeVRPNConnector(const FObjectInitializer& PCIP);
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	void initializeConnection(TArray<FString> aTrackerNames, FString aServerAddress);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	void callMainloop();

	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FVector getBonePosition(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FRotator getBoneRotation(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FQuat getBoneQuaternion(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	float getBoneAcceleration(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	float getBoneVelocity(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FRotator getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint);
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	bool isConnected();

	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static UUnrealMeVRPNConnector* getInstance();
};