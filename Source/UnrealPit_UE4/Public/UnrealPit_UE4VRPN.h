// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealPit_UE4VRPN.generated.h"

/* Structure for a quaternion rotation vector. */
struct QuatVector{
	float x, y, z, w;
};

UCLASS()
class UUnrealPit_UE4VRPN : public UObject
{	
	GENERATED_UCLASS_BODY()

	static void initializeTracker();
	static void callTrackerMainloop();
	static FVector* getKinectBonePosition(int aBoneId);
	static QuatVector* getQuaternion(int aBoneId);
};
