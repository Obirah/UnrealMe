// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeTrackingSystemHelper.h"

TArray<FString> UUnrealMeTrackingSystemHelper::getViconTrackerNames()
{
	TArray<FString> tTrackerNames;

	tTrackerNames.Add(FString(TEXT("torso_unrealme")));
	tTrackerNames.Add(FString(TEXT("lefthand")));
	tTrackerNames.Add(FString(TEXT("righthand")));
	tTrackerNames.Add(FString(TEXT("leftelbow")));
	tTrackerNames.Add(FString(TEXT("rightelbow")));

	return tTrackerNames;
}

FString UUnrealMeTrackingSystemHelper::getViconAddress()
{
	return FString(TEXT("132.187.8.149"));
}

TArray<FString> UUnrealMeTrackingSystemHelper::getFaastTrackerNames()
{
	TArray<FString> tTrackerNames;

	tTrackerNames.Add(FString(TEXT("Tracker0")));

	return tTrackerNames;
}

FString UUnrealMeTrackingSystemHelper::getFaastAddress()
{
	return FString(TEXT("localhost"));
}