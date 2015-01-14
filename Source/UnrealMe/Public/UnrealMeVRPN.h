// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealMeVRPN.generated.h"

/* Structure for a quaternion rotation vector. */
USTRUCT()
struct FQuatVector{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float x;

	UPROPERTY()
	float y;

	UPROPERTY()
	float z;

	UPROPERTY()
	float w;
};

UCLASS(Blueprintable)
class UNREALME_API UUnrealMeVRPN : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "VRPN")
	static void initializeTracker();
	UFUNCTION(BlueprintCallable, Category = "VRPN")
	static void callTrackerMainloop();
	UFUNCTION(BlueprintCallable, Category = "VRPN")
	static FVector getBonePosition(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPN")
	static float getX(int32 aBoneId);
	UFUNCTION(BlueprintCallable, Category = "VRPN")
	static FQuatVector getQuaternion(int32 aBoneId);
};
