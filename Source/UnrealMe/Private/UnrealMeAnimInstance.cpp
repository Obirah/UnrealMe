// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeAnimInstance.h"


UUnrealMeAnimInstance::UUnrealMeAnimInstance(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	//SkelControl_LeftHandPos = FVector(0, 0, 0);
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

	//SkelControl_LeftHandPos = UUnrealMeKinectV2Connector::getJointPosition(7);

	SkelControl_SpineBasePureRot = UUnrealMeKinectV2Connector::getJointRotation(0);
	SkelControl_SpineMidPureRot = UUnrealMeKinectV2Connector::getJointRotation(1);
	SkelControl_NeckPureRot = UUnrealMeKinectV2Connector::getJointRotation(2);

	SkelControl_SpineBasePos = UUnrealMeKinectV2Connector::getJointPosition(0);
	SkelControl_SpineMidPos = UUnrealMeKinectV2Connector::getJointPosition(1);
	SkelControl_NeckPos = UUnrealMeKinectV2Connector::getJointPosition(2);

	SkelControl_TorsoRotationByShoulders = UUnrealMeKinectV2Connector::getJointRotationByPosition(4, 8);
	SkelControl_TorsoRotationByHips = UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 16);

	SkelControl_SpineBaseRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);//UUnrealMeKinectV2Connector::getJointRotation(0);
	SkelControl_SpineMidRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);//UUnrealMeKinectV2Connector::getJointRotation(1);
	SkelControl_NeckRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);//UUnrealMeKinectV2Connector::getJointRotation(2);

	SkelControl_LeftElbowRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(5, 4);//UUnrealMeKinectV2Connector::getJointRotation(5);
	SkelControl_LeftWristRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(6, 5);//UUnrealMeKinectV2Connector::getJointRotation(6);

	SkelControl_RightElbowRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(8, 9);//UUnrealMeKinectV2Connector::getJointRotation(9);
	SkelControl_RightWristRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(9, 10);//UUnrealMeKinectV2Connector::getJointRotation(10);

	SkelControl_LeftKneeRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 13);//UUnrealMeKinectV2Connector::getJointRotation(13);
	SkelControl_LeftAnkleRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(13, 14); //UUnrealMeKinectV2Connector::getJointRotation(14);

	SkelControl_RightKneeRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(17, 16);//UUnrealMeKinectV2Connector::getJointRotation(17);
	SkelControl_RightAnkleRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(18, 17);//UUnrealMeKinectV2Connector::getJointRotation(18);

	
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

/*
void UUnrealMeAnimInstance::setLeftHandPos(FVector aPosition)
{
	SkelControl_LeftHandPos = aPosition;
	}*/