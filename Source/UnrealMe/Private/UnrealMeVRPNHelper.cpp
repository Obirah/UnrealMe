// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeVRPNHelper.h"

UUnrealMeVRPNHelper::UUnrealMeVRPNHelper(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

FVector UUnrealMeVRPNHelper::metersToUnrealUnits(FVector aOriginal)
{
	FVector tBack = FVector(aOriginal.X * 100, aOriginal.Y * 100, aOriginal.Z * 100);
	return tBack;
}

FVector UUnrealMeVRPNHelper::invertAxis(FVector aOriginal, TrackingAxis aSourceAxis, TrackingAxis aTargetAxis)
{
	FVector tBack = aOriginal;

	if ((aSourceAxis == X && aTargetAxis == Y) || (aSourceAxis == Y && aTargetAxis == Z))
	{
		tBack.X = aOriginal.Y;
		tBack.Y = aOriginal.X;
	}
	else if ((aSourceAxis == X && aTargetAxis == Z) || (aSourceAxis == Z && aTargetAxis == Y))
	{
		tBack.X = aOriginal.Z;
		tBack.Z = aOriginal.X;
	}
	else if ((aSourceAxis == Y && aTargetAxis == Z) || (aSourceAxis == Z && aTargetAxis == Y))
	{
		tBack.Y = aOriginal.Z;
		tBack.Z = aOriginal.Y;
	}

	return tBack;
}
