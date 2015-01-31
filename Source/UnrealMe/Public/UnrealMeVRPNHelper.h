// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealMeVRPNHelper.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum TrackingAxis{ X, Y, Z };

UCLASS(Blueprintable)
class UNREALME_API UUnrealMeVRPNHelper : public UObject
{
	GENERATED_BODY()
public:
	UUnrealMeVRPNHelper(const FObjectInitializer& PCIP);
	UFUNCTION(BlueprintCallable, Category = "VRPNHelper")
	static FVector metersToUnrealUnits(FVector aOriginal);
	UFUNCTION(BlueprintCallable, Category = "VRPNHelper")
	static FVector invertAxis(FVector aOriginal, TrackingAxis aSourceAxis, TrackingAxis aTargetAxis);
};
