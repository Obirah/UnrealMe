// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeAnimInstance.h"


UUnrealMeAnimInstance::UUnrealMeAnimInstance(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	SkelControl_LeftHandPos = FVector(0, 0, 0);
	UE_LOG(UnrealMe, Log, TEXT("AnimInstance initialized."));
}

void UUnrealMeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	iOwningPawn = TryGetPawnOwner();

	/*iVRPNClient = NewObject<UUnrealMeVRPN>();
	iVRPNClient->initializeTracker();*/

	UE_LOG(UnrealMe, Log, TEXT("Animation initialized."));
}

void UUnrealMeAnimInstance::NativeUpdateAnimation(float aDeltaTime)
{
	Super::NativeUpdateAnimation(aDeltaTime);

	//if (!iOwningPawn) return;

	SkelControl_LeftHandPos = UUnrealMeKinectV2Connector::getJointPosition(7);
	/*
	iVRPNClient->callTrackerMainloop();

	FVector tHeadPos = iVRPNClient->getBonePosition(0);
	FVector tTorsoPos = iVRPNClient->getBonePosition(2);
	FVector tLHPos = iVRPNClient->getBonePosition(8);
	FVector tRHPos = iVRPNClient->getBonePosition(14);

	FVector tLHPosMultiplied = FVector(tLHPos.X * 1000, tLHPos.Y * 1000, tLHPos.Z * 1000);
	SkelControl_LeftHandPos = tLHPosMultiplied;

	if (tHeadPos.X != 0)
	{
		UE_LOG(UnrealMe, Log, TEXT("Head Position from VRPNClient: %s"), tHeadPos.ToString());
	}
	if (tTorsoPos.X != 0)
	{
		UE_LOG(UnrealMe, Log, TEXT("Torso Position from VRPNClient: %s"), tTorsoPos.ToString());
	}
	if (tLHPos.X != 0)
	{
		UE_LOG(UnrealMe, Log, TEXT("Left Hand Position from VRPNClient: %s"), tLHPos.ToString());
	}
	if (tRHPos.X != 0)
	{
		UE_LOG(UnrealMe, Log, TEXT("Right Hand Position from VRPNClient: %s"), tRHPos.ToString());
	}*/
}

void UUnrealMeAnimInstance::setLeftHandPos(FVector aPosition)
{
	SkelControl_LeftHandPos = aPosition;
}