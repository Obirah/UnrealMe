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
#include "UnrealMeCoordinateHelper.h"
#include "UnrealMeKinectV2Connector.generated.h"

/** Safe release for interfaces */
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
 * Connector Microsoft's Kinect V2
 * The Joint ids:
	 JointType_SpineBase	= 0			JointType_SpineMid	= 1,
	 JointType_Neck	= 2					JointType_Head	= 3,
	 JointType_ShoulderLeft	= 4			JointType_ElbowLeft	= 5,
	 JointType_WristLeft	= 6			JointType_HandLeft	= 7,
	 JointType_ShoulderRight	= 8		JointType_ElbowRight	= 9,
	 JointType_WristRight	= 10		JointType_HandRight	= 11,
	 JointType_HipLeft	= 12			JointType_KneeLeft	= 13,
	 JointType_AnkleLeft	= 14		JointType_FootLeft	= 15,
	 JointType_HipRight	= 16			JointType_KneeRight	= 17,
	 JointType_AnkleRight	= 18		JointType_FootRight	= 19,
	 JointType_SpineShoulder	= 20	JointType_HandTipLeft	= 21,
	 JointType_ThumbLeft	= 22		JointType_HandTipRight	= 23,
	 JointType_ThumbRight	= 24
 */
UCLASS(Blueprintable)
class UNREALME_API UUnrealMeKinectV2Connector : public UObject
{
	GENERATED_BODY()
private:
	
public:
	UUnrealMeKinectV2Connector(const FObjectInitializer& PCIP);
	/* KINECT CONTROL FUNCTIONS FOR THE APPLICATION */

	/** Connect the Kinect. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static void initializeKinect(bool aMultiUser);
	/** High-level update method => does all neccessary internal logic and is used to be called from Unreal's Tick event. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static void update();
	/** Disconnect the Kinect. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static void disconnectKinect();

	/* INTERNAL PROCESSING LOGIC */

	static void initializeTrackingStateMap();
	static void initializeJointToSkeletalBoneMapping();
	/** Checks if the passed torso delta is an outlier (which is defined by the passed threshold). */
	static FVector checkDeltaForOutliers(int32 aThreshold, FVector aDelta);
	/** Update loops for the Kinect data. */
	static void processBody(INT64 aTime, int aBodyCount, IBody** aBodies);
	/** Helper method to update the global collections for multi user tracking. */
	static void updateData(TStaticArray<std::map<int, FVector>, 6> aPositions, TStaticArray<FVector, 6> aDeltas, TStaticArray<CameraSpacePoint, 6> aPreviousTorsoPositions);

	/* SINGLE USER TRACKING FUNCTIONS FOR THE APPLICATIONS */

	/** Get the joint position corresponding to the passed id. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FVector getJointPosition(int32 aJointId);
	/** Get the difference in torso position between time t-1 and t */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FVector getCurrentTorsoDelta();
	/** Get the joint orientation corresponding to the passed id */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FRotator getJointRotation(int32 aJointId);
	/** Derive the joint rotation from the relative position of one joint to another */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FRotator getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint);

	/* MULTI USER TRACKING FUNCTIONS FOR THE APPLICATION */

	/** Get the joint position corresponding to the passed joint id and user id. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FVector getUserJointPosition(int32 aUserId, int32 aJointId);
	/** Get the amount of currently tracked user. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static int32 getTrackedUsersCount();	
	/** Check if the user with the given id is tracked. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static bool isUserTracked(int32 aUserId);
	/** Get the bone name corresponding to the passed id. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FString getBoneNameByJoint(int32 aJointId);	

	/** Experimental function for offsetting purposes. */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static int32 offsetRotation(int32 aAxis, int32 aDegrees);
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
    static void setCurrentTorsoOffset(FVector aTorsoOffset);
};