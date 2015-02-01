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

USTRUCT()
struct FRotations
{
	GENERATED_USTRUCT_BODY()

	TStaticArray<FRotator, 10> iRotationBuffer;

	FRotator getMeanRotator()
	{
		float tSumPitch = 0;
		float tSumRoll = 0;
		float tSumYaw = 0;

		for (int i = 0; i < 10; i++)
		{
			FRotator tCurrentRotator = iRotationBuffer[i];

			tSumPitch += tCurrentRotator.Pitch;
			tSumRoll += tCurrentRotator.Roll;
			tSumYaw += tCurrentRotator.Yaw;
		}

		FRotator tSmoothedRotator = FRotator();
		tSmoothedRotator.Roll = tSumYaw / 10;
		tSmoothedRotator.Pitch = tSumPitch / 10;
		tSmoothedRotator.Yaw = tSumRoll / 10;

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
	GENERATED_BODY()
private:
	
public:
	UUnrealMeKinectV2Connector(const FObjectInitializer& PCIP);
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
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FRotator getJointRotation(int32 aJointId);

	static FRotator getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint);

	/* MULTI USER TRACKING FUNCTIONS FOR THE APPLICATION */
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FVector getUserJointPosition(int32 aUserId, int32 aJointId);
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static int32 getTrackedUsersCount();	
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static bool isUserTracked(int32 aUserId);
	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static FString getBoneNameByJoint(int32 aJointId);	

	UFUNCTION(BlueprintCallable, Category = "KinectV2Connector")
	static int32 offsetRotation(int32 aAxis, int32 aDegrees);
};