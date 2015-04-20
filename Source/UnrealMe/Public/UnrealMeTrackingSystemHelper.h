// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealMeTrackingSystemHelper.generated.h"
/**
 * 
 */
UCLASS()
class UNREALME_API UUnrealMeTrackingSystemHelper : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static TArray<FString> getViconTrackerNames();
	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static FString getViconAddress();

	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static TArray<FString> getFaastTrackerNames();
	UFUNCTION(BlueprintCallable, Category = "TrackingSystemHelper")
	static FString getFaastAddress();
};
