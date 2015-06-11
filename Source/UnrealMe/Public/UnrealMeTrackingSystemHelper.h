// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealMeTrackingSystemHelper.generated.h"
/**
 * Helper class that returns values for tracking systems to be used and connected to.
 */
UCLASS()
class UNREALME_API UUnrealMeTrackingSystemHelper : public UObject
{
	GENERATED_BODY()
public:
	/** Returns the tracker names for the HCI group's Vicon tracking system */
	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static TArray<FString> getViconTrackerNames();
	/** Returns the address for the HCI group's Vicon tracking system */
	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static FString getViconAddress();
	/** Returns the tracker names for the FAAST VRPN server */
	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static TArray<FString> getFaastTrackerNames();
	/** Returns the address for the FAAST VRPN server */
	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static FString getFaastAddress();
};
