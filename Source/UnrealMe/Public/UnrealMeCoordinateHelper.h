// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AllowWindowsPlatformTypes.h"
#include <Kinect.h>
#include "HideWindowsPlatformTypes.h"

/**
 * Class containing helper functions to convert coordinates and rotations between different spaces
 * and systems. This class was and is continous work in progress, as some transformations (e.g. the
 * conversion of rotations to Unreal) will have to be adjusted further in the future.
 */
class UNREALME_API UUnrealMeCoordinateHelper
{
public:
	/** Convert absolute position to relative position */
	static CameraSpacePoint getRelativePosition(CameraSpacePoint aTorso, CameraSpacePoint aOtherJoint);

	/**
	* Conversion from Kinect to Unreal coordinate system:
	* Kinect X	=> Unreal -Y
	* Kinect Y => Unreal -Z
	* Kinect Z => Unreal X
	*/
	static FVector convertPositionToUnrealSpace(CameraSpacePoint aPosition);

	/** Conversion of a position in three floating coordinates to the Unreal space */
	static FVector convertPositionToUnrealSpace(float aX, float aY, float aZ);

	/**
	* Conversion from Unreal to Kinect coordinate system:
	* Unreal -Y => Kinect X
	* Unreal -Z => Kinect Y 
	* Unreal X => Kinect Z 
	*/
	static CameraSpacePoint convertPositionToKinectSpace(FVector aPosition);

	/** Convert a rotation (e.g. from VRPN) to the Unreal space */
	static FRotator convertRotationToUnrealSpace(FRotator aRotator);

	/** Convert a quaternion rotation from Kinect to Unreal space. - Vector4 version */
	static FQuat convertQuatRotationToUnrealSpace(Vector4 aQuaternion);

	/** Convert a quaternion rotation from Kinect to Unreal space. - float version */
	static FQuat convertQuatRotationToUnrealSpace(float aX, float aY, float aZ, float aW);

	/** Convert a Vector4 quaternion to a Unreal rotator */
	static FRotator convertQuatRotationToRotator(Vector4 aQuaternion);

	/* Create a quaternion rotation from two vectors */
	static FQuat getQuaternionFromVectors(FVector aVector1, FVector aVector2);

	/** Convert a FQuat quaternion to a Unreal rotator */
	static FRotator convertQuatRotationToRotator(FQuat aQuaternion, int32 aHomogenous);

	/** Convert a Vector4 quaternion to euler angels. */
	static FVector QuaternionToEuler(Vector4 aQuaternion);

	/** Convert radians to degrees */
	static double RadianToDegree(double aAngle);
};
