// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeCoordinateHelper.h"

/* Factor to convert the metric Kinect values to Unreal units (= cm) */
static const int KINECT_TO_UNREAL_FACTOR = 100;

CameraSpacePoint UUnrealMeCoordinateHelper::getRelativePosition(CameraSpacePoint aTorso, CameraSpacePoint aOtherJoint)
{
	CameraSpacePoint tBack = CameraSpacePoint();

	tBack.X = aTorso.X - aOtherJoint.X;
	tBack.Y = aTorso.Y - aOtherJoint.Y;
	tBack.Z = aTorso.Z - aOtherJoint.Z;

	return tBack;
}

FVector UUnrealMeCoordinateHelper::convertPositionToUnrealSpace(CameraSpacePoint aPosition)
{
	return FVector(aPosition.Z * KINECT_TO_UNREAL_FACTOR, aPosition.X * -KINECT_TO_UNREAL_FACTOR, aPosition.Y * -KINECT_TO_UNREAL_FACTOR);
}

FVector UUnrealMeCoordinateHelper::convertPositionToUnrealSpace(float aX, float aY, float aZ)
{
	CameraSpacePoint tCameraSpacePoint = CameraSpacePoint();

	tCameraSpacePoint.X = aX;
	tCameraSpacePoint.Y = aY;
	tCameraSpacePoint.Z = aZ;

	return convertPositionToUnrealSpace(tCameraSpacePoint);
}

CameraSpacePoint UUnrealMeCoordinateHelper::convertPositionToKinectSpace(FVector aPosition)
{
	CameraSpacePoint tBack = CameraSpacePoint();

	tBack.X = aPosition.Y / -KINECT_TO_UNREAL_FACTOR;
	tBack.Y = aPosition.Z / -KINECT_TO_UNREAL_FACTOR;
	tBack.Z = aPosition.X / KINECT_TO_UNREAL_FACTOR;

	return tBack;
}

FRotator UUnrealMeCoordinateHelper::convertRotationToUnrealSpace(FRotator aRotator)
{
	FRotator tBack = FRotator();

	tBack.Roll = aRotator.Pitch;
	tBack.Pitch = aRotator.Yaw * -1;
	tBack.Yaw = aRotator.Roll;

	return tBack;
}

FQuat UUnrealMeCoordinateHelper::convertQuatRotationToUnrealSpace(Vector4 aQuaternion)
{
	return FQuat(aQuaternion.z, aQuaternion.x, aQuaternion.y, aQuaternion.w);
}

FQuat UUnrealMeCoordinateHelper::convertQuatRotationToUnrealSpace(float aX, float aY, float aZ, float aW)
{
	Vector4 tVector4;

	tVector4.x = aX;
	tVector4.y = aY;
	tVector4.z = aZ;
	tVector4.w = aW;

	return convertQuatRotationToUnrealSpace(tVector4);
}

FQuat UUnrealMeCoordinateHelper::getQuaternionFromVectors(FVector aVector1, FVector aVector2)
{
	FQuat tQuaternion = FQuat();
	FVector tCrossProduct = FVector::CrossProduct(aVector1, aVector2);

	tQuaternion.X = tCrossProduct.X;
	tQuaternion.Y = tCrossProduct.Y;
	tQuaternion.Z = tCrossProduct.Z;

	float tV1size = aVector1.Size();
	float tV2size = aVector2.Size();

	float tQuatW = FMath::Sqrt(FMath::Pow(tV1size, 2) * FMath::Pow(tV2size, 2)) + FVector::DotProduct(aVector1, aVector2);
	tQuaternion.W = tQuatW;

	tQuaternion.Normalize();
	return tQuaternion;
}

FRotator UUnrealMeCoordinateHelper::convertQuatRotationToRotator(Vector4 aQuaternion)
{
	FVector tRotation = QuaternionToEuler(aQuaternion);

	FRotator tBack = FRotator();
	tBack.Roll = tRotation.Z;
	tBack.Yaw = tRotation.Y;
	tBack.Pitch = tRotation.X * -1;

	return tBack;
}

FRotator UUnrealMeCoordinateHelper::convertQuatRotationToRotator(FQuat aQuaternion, int32 aHomogenous)
{
	float sqx = aQuaternion.X * aQuaternion.X;
	float sqy = aQuaternion.Y * aQuaternion.Y;
	float sqz = aQuaternion.Z * aQuaternion.Z;
	float sqw = aQuaternion.W * aQuaternion.W;

	FRotator tBack;

	if (aHomogenous == 1){
		tBack.Roll = atan2f(2.f*(aQuaternion.X*aQuaternion.Y + aQuaternion.Z * aQuaternion.W), sqx - sqy - sqz + sqw);
		tBack.Pitch = asinf(-2.f*(aQuaternion.X*aQuaternion.Z - aQuaternion.Y * aQuaternion.W));
		tBack.Yaw = atan2f(2.f*(aQuaternion.Y*aQuaternion.Z + aQuaternion.X * aQuaternion.W), -sqx - sqy + sqz + sqw);
	}
	else{
		tBack.Roll = atan2f(2.f * (aQuaternion.Z * aQuaternion.Y + aQuaternion.X * aQuaternion.W), 1 - 2 * (sqx + sqy));
		tBack.Pitch = asinf(-2.f * (aQuaternion.X * aQuaternion.Z - aQuaternion.Y * aQuaternion.W));
		tBack.Yaw = atan2f(2.f * (aQuaternion.X * aQuaternion.Y + aQuaternion.Z * aQuaternion.W), 1 - 2 * (sqy + sqz));
	}

	return tBack;
}

FVector UUnrealMeCoordinateHelper::QuaternionToEuler(Vector4 aQuaternion)
{
	FVector tVector = FVector(0, 0, 0);

	// If the Z, pitch, attitude is straight up or straight down, Y, roll, bank is zero
	// X, Yaw, heading is + or - the ... umm... the angle computed from Atan2 of X,W
	// Basically, from what I understand, this choses Heading, Yaw or X
	// when faced with gimbal lock and zeros Bank, Roll or Y
	if (aQuaternion.x * aQuaternion.y + aQuaternion.z * aQuaternion.w == 0.5)
	{
		//X Angle represents Yaw, Heading
		tVector.X = (2 * FMath::Atan2(aQuaternion.x, aQuaternion.w));
		//Y Angle represents Roll, Bank
		tVector.Y = 0;
	}
	else if (aQuaternion.x * aQuaternion.y + aQuaternion.z * aQuaternion.w == -0.5)
	{
		//X Angle represents Yaw, Heading
		tVector.X = (-2 * FMath::Atan2(aQuaternion.x, aQuaternion.w));
		//Y Angle represents Roll, Bank
		tVector.Y = 0;
	}
	else
	{
		//X Angle represents Yaw, heading 
		tVector.X = FMath::Atan2(2 * aQuaternion.y * aQuaternion.w - 2 * aQuaternion.x * aQuaternion.z,
			1 - 2 * FMath::Pow(aQuaternion.y, 2) - 2 * FMath::Pow(aQuaternion.z, 2));

		//Y Angle represents Roll, bank
		tVector.Y = FMath::Atan2(2 * aQuaternion.x * aQuaternion.w - 2 * aQuaternion.y * aQuaternion.z,
			1 - 2 * FMath::Pow(aQuaternion.x, 2) - 2 * FMath::Pow(aQuaternion.z, 2));
	}

	//Z Angle represents Pitch, attitude
	tVector.Z = FMath::Asin(2 * aQuaternion.x * aQuaternion.y + 2 * aQuaternion.z * aQuaternion.w);


	//Convert the Euler angles from Radians to Degrees
	tVector.X = RadianToDegree(tVector.X);
	tVector.Y = RadianToDegree(tVector.Y);
	tVector.Z = RadianToDegree(tVector.Z);
	return tVector;
}

double UUnrealMeCoordinateHelper::RadianToDegree(double aAngle)
{
	//Return degrees (0->360) from radians
	return aAngle * (180.0 / PI) + 180;
}
