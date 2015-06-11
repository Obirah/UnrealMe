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
* VRPN client class
* Bone IDs:
	0 	Head 				12 	Right Elbow
	1 	Neck 				13 	Right Wrist
	2 	Torso 				14 	Right Hand
	3 	Waist 				15 	Right Fingertip
	4 	Left Collar 		16 	Left Hip
	5 	Left Shoulder 		17 	Left Knee
	6 	Left Elbow 			18 	Left Ankle
	7 	Left Wrist 			19 	Left Foot
	8 	Left Hand 			20 	Right Hip
	9 	Left Fingertip 		21 	Right Knee
	10 	Right Collar 		22 	Right Ankle
	11 	Right Shoulder 		23 	Right Foot
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
	/** Default constrcutor */
	UUnrealMeVRPNConnector(const FObjectInitializer& PCIP);
	/** Overriden begin destroy to implement functionality when the Unreal garbage collector collects. */
	virtual void BeginDestroy() override;

	/** Connect to VRPN using the specified tracker names and server address. */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	void initializeConnection(TArray<FString> aTrackerNames, FString aServerAddress);
	/** Disconnect from VRPN. */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	void disconnect();
	/** Call the mainloops of all currently used tracker remotes. */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	void callMainloop();

	/** Returns the position of the specified bone */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FVector getBonePosition(int32 aBoneId);
	/** Returns the rotation of the specified bone */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FRotator getBoneRotation(int32 aBoneId);
	/** Returns the quaternion rotation of the specified bone */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FQuat getBoneQuaternion(int32 aBoneId);
	/** Returns the acceleration of the specified bone */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	float getBoneAcceleration(int32 aBoneId);
	/** Returns the velocity of the specified bone */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	float getBoneVelocity(int32 aBoneId);
	/** Returns the rotation of a bone calculated by the relative locations of the two specified bones */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	FRotator getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint);
	/** Returns the flag indicating whether a connection is established */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	bool isConnected();
	/** Returns the singleton instance */
	UFUNCTION(BlueprintCallable, Category = "VRPNConnector")
	static UUnrealMeVRPNConnector* getInstance();
};