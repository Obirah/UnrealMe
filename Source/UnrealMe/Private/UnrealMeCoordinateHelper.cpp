// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeCoordinateHelper.h"

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
	return FVector(aPosition.Z * 100, aPosition.X * -100, aPosition.Y * -100);
}

CameraSpacePoint UUnrealMeCoordinateHelper::convertPositionToKinectSpace(FVector aPosition)
{
	CameraSpacePoint tBack = CameraSpacePoint();

	tBack.X = aPosition.Y / -100;
	tBack.Y = aPosition.Z / -100;
	tBack.Z = aPosition.X / 100;

	return tBack;
}

FQuat UUnrealMeCoordinateHelper::convertRotationToUnrealSpace(Vector4 aQuaternion)
{
	return FQuat(aQuaternion.z, aQuaternion.x, aQuaternion.y, aQuaternion.w);
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
