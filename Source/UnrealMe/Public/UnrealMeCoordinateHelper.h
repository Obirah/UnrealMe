// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AllowWindowsPlatformTypes.h"
#include <Kinect.h>
#include "HideWindowsPlatformTypes.h"

/**
 * 
 */
class UNREALME_API UUnrealMeCoordinateHelper
{
public:
	/* Convert absolute position to relative position */
	static CameraSpacePoint getRelativePosition(CameraSpacePoint aTorso, CameraSpacePoint aOtherJoint);

	/*
	* Conversion from Kinect to Unreal coordinate system:
	* Kinect X	=> Unreal -Y
	* Kinect Y => Unreal -Z
	* Kinect Z => Unreal X
	*/
	static FVector convertPositionToUnrealSpace(CameraSpacePoint aPosition);

	static FVector convertPositionToUnrealSpace(float aX, float aY, float aZ);

	/*
	* Conversion from Unreal to Kinect coordinate system:
	* Unreal -Y => Kinect X
	* Unreal -Z => Kinect Y 
	* Unreal X => Kinect Z 
	*/
	static CameraSpacePoint convertPositionToKinectSpace(FVector aPosition);

	static FRotator convertRotationToUnrealSpace(FRotator aRotator);

	/* Convert a quaternion rotation from Kinect to Unreal space. */
	static FQuat convertQuatRotationToUnrealSpace(Vector4 aQuaternion);

	static FQuat convertQuatRotationToUnrealSpace(float aX, float aY, float aZ, float aW);

	static FRotator convertQuatRotationToRotator(Vector4 aQuaternion);

	/* Create a quaternion rotation from two vectors. */
	static FQuat getQuaternionFromVectors(FVector aVector1, FVector aVector2);

	static FRotator convertQuatRotationToRotator(FQuat aQuaternion, int32 aHomogenous);

	static FVector QuaternionToEuler(Vector4 q);

	static double RadianToDegree(double angle);
};
