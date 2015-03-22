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

/** Experimental struct for future data structure optimizations. */
USTRUCT()
struct FRotations
{
	GENERATED_USTRUCT_BODY()

	TStaticArray<FRotator, 3> iRotationBuffer;

	FRotator getMeanRotator()
	{
		float tSumPitch = 0;
		float tSumRoll = 0;
		float tSumYaw = 0;

		for (int i = 0; i < 3; i++)
		{
			FRotator tCurrentRotator = iRotationBuffer[i];

			tSumPitch += tCurrentRotator.Pitch;
			tSumRoll += tCurrentRotator.Roll;
			tSumYaw += tCurrentRotator.Yaw;
		}

		FRotator tSmoothedRotator = FRotator();
		tSmoothedRotator.Roll = tSumRoll / 3;
		tSmoothedRotator.Pitch = tSumPitch / 3;
		tSmoothedRotator.Yaw = tSumYaw / 3;

		return tSmoothedRotator;
	}

	void addRotatorAt(int32 aIndex, FRotator aRotator)
	{
		iRotationBuffer[aIndex] = aRotator;
	}

	FRotations()
	{
		
	}
};

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
 * UCLASS definition => usable in Blueprints.
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