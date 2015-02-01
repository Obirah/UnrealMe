// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeAnimInstance.h"


UUnrealMeAnimInstance::UUnrealMeAnimInstance(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	UE_LOG(UnrealMe, Log, TEXT("AnimInstance initialized."));
}

void UUnrealMeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	iOwningPawn = TryGetPawnOwner();

	UE_LOG(UnrealMe, Log, TEXT("Animation initialized."));
}

void UUnrealMeAnimInstance::NativeUpdateAnimation(float aDeltaTime)
{
	Super::NativeUpdateAnimation(aDeltaTime);

	SkelControl_SpineBasePureRot = UUnrealMeKinectV2Connector::getJointRotation(0);
	SkelControl_SpineMidPureRot = UUnrealMeKinectV2Connector::getJointRotation(1);
	SkelControl_NeckPureRot = UUnrealMeKinectV2Connector::getJointRotation(2);

	SkelControl_SpineBasePos = UUnrealMeKinectV2Connector::getJointPosition(0);
	SkelControl_SpineMidPos = UUnrealMeKinectV2Connector::getJointPosition(1);
	SkelControl_NeckPos = UUnrealMeKinectV2Connector::getJointPosition(2);

	SkelControl_TorsoRotationByShoulders = UUnrealMeKinectV2Connector::getJointRotationByPosition(4, 8);
	SkelControl_TorsoRotationByHips = UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 16);

	SkelControl_SpineBaseRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);
	SkelControl_SpineMidRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);
	SkelControl_NeckRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);

	SkelControl_LeftUpperarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(5, 4);
	SkelControl_LeftLowerarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(6, 5);
	SkelControl_LeftHandRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(7, 6);

	SkelControl_RightUpperarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(8, 9);
	SkelControl_RightLowerarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(9, 10);
	SkelControl_RightHandRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(10, 11);

	SkelControl_LeftThighRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 13);
	SkelControl_LeftCalfRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(13, 14);
	SkelControl_LeftFootRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(14, 15);

	SkelControl_RightThighRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(17, 16);
	SkelControl_RightCalfRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(18, 17);
	SkelControl_RightFootRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(19, 18);
}