// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#include "AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <Kinect.h>
#include <map>
#include "HideWindowsPlatformTypes.h"
#include "UnrealMeKinectV2Connector.generated.h"

/* Safe release for interfaces */
template<class Interface>
inline void SafeRelease(Interface *& aInterfaceToRelease)
{
	if (aInterfaceToRelease != NULL)
	{
		aInterfaceToRelease->Release();
		aInterfaceToRelease = NULL;
	}
}

/**
 * UCLASS definition => usable in Blueprints.
 */
UCLASS(Blueprintable)
class UNREALME_API UUnrealMeKinectV2Connector : public UObject
{
	GENERATED_UCLASS_BODY()
private:
	/* COLLECTIONS */
	static TStaticArray<std::map<int, FVector>, 6> iUsersSkeletonData;
	static std::map<int, FVector> iSkeletonData;
	static std::map<int, bool> iUserTrackingState;
	static std::map<int, FString> iJointToSkeletalBone;

	/* TORSO POSITIONS AT TIME T-1 */
	static CameraSpacePoint iPreviousTorsoPos;
	static std::map<int, CameraSpacePoint> iUsersPreviousTorsoPos;
	/* TORSO POSITIONS AT TIME T */
	static FVector iCurrentTorsoDelta;
	static std::map<int, FVector> iUsersTorsoDeltas;

	/* KINECT VARIABLES */
	static IKinectSensor* iKinectSensor;
	static ICoordinateMapper* iCoordinateMapper;
	static IBodyFrameReader* iBodyFrameReader;

	/* MISCELLANEOUS */
	static int iTrackedUsers;
	static bool iMultiUser;
public:
	/* KINECT CONTROL FUNCTIONS FOR THE APPLICATION */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static void initializeKinect(bool aMultiUser);
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static void update();
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static void disconnectKinect();

	/* INTERNAL PROCESSING LOGIC */
	static void initializeTrackingStateMap();
	static void initializeJointToSkeletalBoneMapping();
	static FVector checkDeltaForOutliers(int32 aThreshold, FVector aDelta);
	static void processBody(INT64 aTime, int aBodyCount, IBody** aBodies);
	static void updateData(TStaticArray<std::map<int, FVector>, 6> aPositions, TStaticArray<FVector, 6> aDeltas, TStaticArray<CameraSpacePoint, 6> aPreviousTorsoPositions);

	/* SINGLE USER TRACKING FUNCTIONS FOR THE APPLICATIONS */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FVector getJointPosition(int32 aJointId);
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
		static FVector getCurrentTorsoDelta();

	/* MULTI USER TRACKING FUNCTIONS FOR THE APPLICATION */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FVector getUserJointPosition(int32 aUserId, int32 aJointId);
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static int32 getTrackedUsersCount();	
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static bool isUserTracked(int32 aUserId);
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FString getBoneNameByJoint(int32 aJointId);	
};