// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeAnimInstance.h"

int32 iFrameCount = 0;
TStaticArray<FRotations, 14> iBuffer;
bool iBuffered = false;
static std::map<EJointRotationType, FRotator> iRotations;

UUnrealMeAnimInstance::UUnrealMeAnimInstance(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	UE_LOG(UnrealMe, Log, TEXT("AnimInstance initialized."));
}

void UUnrealMeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	iOwningPawn = TryGetPawnOwner();

	if (iBuffered)
	{
		for (int i = 0; i < 11; i++)
		{
			iBuffer[i] = FRotations();
		}
	}

	UE_LOG(UnrealMe, Log, TEXT("Animation initialized."));
}

void UUnrealMeAnimInstance::NativeUpdateAnimation(float aDeltaTime)
{
	Super::NativeUpdateAnimation(aDeltaTime);

	/* Currently stable method. */
	if (!iBuffered)
	{		
		/* Updated data structure to be accessed via Enum to come. */
		//std::map<EJointRotationType, FRotator> tRotations;
		//tRotations[EJointRotationType::JR_SPINE_BY_S] = UUnrealMeKinectV2Connector::getJointRotationByPosition(4, 8);

		/* Update the core */
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

		/* Update left arm */
		SkelControl_LeftUpperarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(5, 4);
		SkelControl_LeftLowerarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(6, 5);
		SkelControl_LeftHandRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(7, 6);

		SkelControl_LeftHandTipRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(21, 7);
		SkelControl_LeftThumbRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(22, 7);	

		/* Update right arm */
		SkelControl_RightUpperarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(8, 9);
		SkelControl_RightLowerarmRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(9, 10);
		SkelControl_RightHandRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(10, 11);

		SkelControl_RightHandTipRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(11, 23);
		SkelControl_RightThumbRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(11, 24);

		/* Update left leg */
		SkelControl_LeftThighRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 13);
		SkelControl_LeftCalfRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(13, 14);
		SkelControl_LeftFootRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(14, 15);

		/* Update right leg */
		SkelControl_RightThighRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(17, 16);
		SkelControl_RightCalfRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(18, 17);
		SkelControl_RightFootRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(19, 18);

		//iRotations = tRotations;
	}
	/* Exploration of smoothing the rotations. */
	else
	{
		iBuffer[0].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(5, 4));
		iBuffer[1].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(6, 5));
		iBuffer[2].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(7, 6));

		iBuffer[3].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(8, 9));
		iBuffer[4].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(9, 10));
		iBuffer[5].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(10, 11));

		iBuffer[6].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 13));
		iBuffer[7].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(13, 14));
		iBuffer[8].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(14, 15));

		iBuffer[9].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(17, 16));
		iBuffer[10].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(18, 17));
		iBuffer[11].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(19, 18));

		iBuffer[12].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(4, 8));
		iBuffer[13].addRotatorAt(iFrameCount, UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 16));

		if (iFrameCount < 2)
		{
			iFrameCount++;
		}
		else
		{	
			SkelControl_SpineBasePureRot = UUnrealMeKinectV2Connector::getJointRotation(0);
			SkelControl_SpineMidPureRot = UUnrealMeKinectV2Connector::getJointRotation(1);
			SkelControl_NeckPureRot = UUnrealMeKinectV2Connector::getJointRotation(2);

			SkelControl_SpineBasePos = UUnrealMeKinectV2Connector::getJointPosition(0);
			SkelControl_SpineMidPos = UUnrealMeKinectV2Connector::getJointPosition(1);
			SkelControl_NeckPos = UUnrealMeKinectV2Connector::getJointPosition(2);

			SkelControl_TorsoRotationByShoulders = iBuffer[12].getMeanRotator();
			SkelControl_TorsoRotationByHips = iBuffer[13].getMeanRotator();

			SkelControl_SpineBaseRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);
			SkelControl_SpineMidRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);
			SkelControl_NeckRot = UUnrealMeKinectV2Connector::getJointRotationByPosition(0, 2);

			SkelControl_LeftUpperarmRot = iBuffer[0].getMeanRotator();
			SkelControl_LeftLowerarmRot = iBuffer[1].getMeanRotator();
			SkelControl_LeftHandRot = iBuffer[2].getMeanRotator();

			SkelControl_RightUpperarmRot = iBuffer[3].getMeanRotator();
			SkelControl_RightLowerarmRot = iBuffer[4].getMeanRotator();
			SkelControl_RightHandRot = iBuffer[5].getMeanRotator();

			SkelControl_LeftThighRot = iBuffer[6].getMeanRotator();
			SkelControl_LeftCalfRot = iBuffer[7].getMeanRotator();
			SkelControl_LeftFootRot = iBuffer[8].getMeanRotator();

			SkelControl_RightThighRot = iBuffer[9].getMeanRotator();
			SkelControl_RightCalfRot = iBuffer[10].getMeanRotator();
			SkelControl_RightFootRot = iBuffer[11].getMeanRotator();

			iFrameCount = 0;
		}
	}	
}

FRotator UUnrealMeAnimInstance::getJointRotation(EJointRotationType aJoint)
{
	return iRotations[aJoint];
}