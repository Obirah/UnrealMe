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
	iVrpnConnector = UUnrealMeVRPNConnector::getInstance();
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

	if (iVrpn && iVrpnConnector->isConnected() == false)
	{
		iVrpnConnector->initializeConnection(UUnrealMeTrackingSystemHelper::getViconTrackerNames(), UUnrealMeTrackingSystemHelper::getViconAddress());
	}

	UE_LOG(UnrealMe, Log, TEXT("Animation initialized."));
}

void UUnrealMeAnimInstance::NativeUpdateAnimation(float aDeltaTime)
{
	Super::NativeUpdateAnimation(aDeltaTime);

	/*if (iVrpn)
	{
		iVrpnConnector->callMainloop();
	}*/

	/* Currently stable method. */
	if (!iBuffered)
	{		
		/* Updated data structure to be accessed via Enum to come. */
		//std::map<EJointRotationType, FRotator> tRotations;
		//tRotations[EJointRotationType::JR_SPINE_BY_S] = UUnrealMeKinectV2Connector::getJointRotationByPosition(4, 8);

		/* Update the core */
		if (iVrpn && iVrpnConnector->isConnected())
		{
			SkelControl_SpineBasePureRot = iVrpnConnector->getBoneRotation(0);
			SkelControl_SpineMidPureRot = iVrpnConnector->getBoneRotation(0);
			SkelControl_NeckPureRot = iVrpnConnector->getBoneRotation(0);

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TorsoRotation: %f, %f, %f"), SkelControl_SpineMidPureRot.Pitch, SkelControl_SpineMidPureRot.Roll, SkelControl_SpineMidPureRot.Yaw));
			/*
			SkelControl_SpineBasePureRot = UUnrealMeVRPNConnector::getBoneRotation(3);
			SkelControl_SpineMidPureRot = UUnrealMeVRPNConnector::getBoneRotation(2);
			SkelControl_NeckPureRot = UUnrealMeVRPNConnector::getBoneRotation(1);

			SkelControl_SpineBasePos = UUnrealMeVRPNConnector::getBonePosition(3);
			SkelControl_SpineMidPos = UUnrealMeVRPNConnector::getBonePosition(2);
			SkelControl_NeckPos = UUnrealMeVRPNConnector::getBonePosition(1);
			*/
		}
		else
		{
			SkelControl_SpineBasePureRot = UUnrealMeKinectV2Connector::getJointRotation(0);
			SkelControl_SpineMidPureRot = UUnrealMeKinectV2Connector::getJointRotation(1);
			SkelControl_NeckPureRot = UUnrealMeKinectV2Connector::getJointRotation(2);

			SkelControl_SpineBasePos = UUnrealMeKinectV2Connector::getJointPosition(0);
			SkelControl_SpineMidPos = UUnrealMeKinectV2Connector::getJointPosition(1);
			SkelControl_NeckPos = UUnrealMeKinectV2Connector::getJointPosition(2);
		}		

		if (SkelControl_UsePureRotations)
		{
			if (iVrpn && iVrpnConnector->isConnected())
			{
				updateRotationsVRPN();
			}
			else
			{
				updateRotations();
			}			
		}
		else
		{
			if (iVrpn && iVrpnConnector->isConnected())
			{
				updateRotationsByPositionsVRPN();
			}
			else
			{
				updateRotationsByPositions();
			}			
		}
		

		//iRotations = tRotations;
	}
	/* Exploration of smoothing the rotations. */
	else
	{
		updateRotationsByPositionsWithSmoothing();
	}		
}

void UUnrealMeAnimInstance::updateRotations()
{
	/* Need to be set so that this method is corresponding to updateRotationsByPositions */
	SkelControl_TorsoRotationByShoulders = UUnrealMeKinectV2Connector::getJointRotation(2);
	SkelControl_TorsoRotationByHips = UUnrealMeKinectV2Connector::getJointRotation(0);

	SkelControl_SpineBaseRot = UUnrealMeKinectV2Connector::getJointRotation(0);
	SkelControl_SpineMidRot = UUnrealMeKinectV2Connector::getJointRotation(1);
	SkelControl_NeckRot = UUnrealMeKinectV2Connector::getJointRotation(2);

	/* Update left arm */
	SkelControl_LeftUpperarmRot = UUnrealMeKinectV2Connector::getJointRotation(5);
	SkelControl_LeftLowerarmRot = UUnrealMeKinectV2Connector::getJointRotation(6);
	SkelControl_LeftHandRot = UUnrealMeKinectV2Connector::getJointRotation(7);

	SkelControl_LeftHandTipRot = UUnrealMeKinectV2Connector::getJointRotation(21);
	SkelControl_LeftThumbRot = UUnrealMeKinectV2Connector::getJointRotation(22);

	/* Update right arm */
	SkelControl_RightUpperarmRot = UUnrealMeKinectV2Connector::getJointRotation(9);
	SkelControl_RightLowerarmRot = UUnrealMeKinectV2Connector::getJointRotation(10);
	SkelControl_RightHandRot = UUnrealMeKinectV2Connector::getJointRotation(11);

	SkelControl_RightHandTipRot = UUnrealMeKinectV2Connector::getJointRotation(23);
	SkelControl_RightThumbRot = UUnrealMeKinectV2Connector::getJointRotation(24);

	/* Update left leg */
	SkelControl_LeftThighRot = UUnrealMeKinectV2Connector::getJointRotation(13);
	SkelControl_LeftCalfRot = UUnrealMeKinectV2Connector::getJointRotation(14);
	SkelControl_LeftFootRot = UUnrealMeKinectV2Connector::getJointRotation(15);

	/* Update right leg */
	SkelControl_RightThighRot = UUnrealMeKinectV2Connector::getJointRotation(17);
	SkelControl_RightCalfRot = UUnrealMeKinectV2Connector::getJointRotation(18);
	SkelControl_RightFootRot = UUnrealMeKinectV2Connector::getJointRotation(19);
}

void UUnrealMeAnimInstance::updateRotationsByPositions()
{
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
}

void UUnrealMeAnimInstance::updateRotationsVRPN()
{
	SkelControl_LeftHandRot = iVrpnConnector->getBoneRotation(1);
	SkelControl_RightHandRot = iVrpnConnector->getBoneRotation(2);

	SkelControl_LeftLowerarmRot = iVrpnConnector->getBoneRotation(3);
	SkelControl_RightLowerarmRot = iVrpnConnector->getBoneRotation(4);

	/* Need to be set so that this method is corresponding to updateRotationsByPositions 
	SkelControl_TorsoRotationByShoulders = UUnrealMeVRPNConnector::getBoneRotation(1);
	SkelControl_TorsoRotationByHips = UUnrealMeVRPNConnector::getBoneRotation(3);

	SkelControl_SpineBaseRot = UUnrealMeVRPNConnector::getBoneRotation(3);
	SkelControl_SpineMidRot = UUnrealMeVRPNConnector::getBoneRotation(2);
	SkelControl_NeckRot = UUnrealMeVRPNConnector::getBoneRotation(0);*/

	/* Update left arm 
	SkelControl_LeftUpperarmRot = UUnrealMeVRPNConnector::getBoneRotation(6);
	SkelControl_LeftLowerarmRot = UUnrealMeVRPNConnector::getBoneRotation(7);
	SkelControl_LeftHandRot = UUnrealMeVRPNConnector::getBoneRotation(8);

	SkelControl_LeftHandTipRot = UUnrealMeVRPNConnector::getBoneRotation(9);
	SkelControl_LeftThumbRot = UUnrealMeVRPNConnector::getBoneRotation(9);*/

	/* Update right arm 
	SkelControl_RightUpperarmRot = UUnrealMeVRPNConnector::getBoneRotation(12);
	SkelControl_RightLowerarmRot = UUnrealMeVRPNConnector::getBoneRotation(13);
	SkelControl_RightHandRot = UUnrealMeVRPNConnector::getBoneRotation(14);

	SkelControl_RightHandTipRot = UUnrealMeVRPNConnector::getBoneRotation(15);
	SkelControl_RightThumbRot = UUnrealMeVRPNConnector::getBoneRotation(15);*/

	/* Update left leg 
	SkelControl_LeftThighRot = UUnrealMeVRPNConnector::getBoneRotation(16);
	SkelControl_LeftCalfRot = UUnrealMeVRPNConnector::getBoneRotation(17);
	SkelControl_LeftFootRot = UUnrealMeVRPNConnector::getBoneRotation(18);*/

	/* Update right leg 
	SkelControl_RightThighRot = UUnrealMeVRPNConnector::getBoneRotation(20);
	SkelControl_RightCalfRot = UUnrealMeVRPNConnector::getBoneRotation(21);
	SkelControl_RightFootRot = UUnrealMeVRPNConnector::getBoneRotation(22); */
	
}

void UUnrealMeAnimInstance::updateRotationsByPositionsVRPN()
{
	SkelControl_TorsoRotationByShoulders = iVrpnConnector->getJointRotationByPosition(5, 11);// UUnrealMeKinectV2Connector::getJointRotationByPosition(4, 8);
	SkelControl_TorsoRotationByHips = iVrpnConnector->getJointRotationByPosition(16, 20); //UUnrealMeKinectV2Connector::getJointRotationByPosition(12, 16);

	SkelControl_SpineBaseRot = iVrpnConnector->getJointRotationByPosition(2, 1);
	SkelControl_SpineMidRot = iVrpnConnector->getJointRotationByPosition(2, 1);
	SkelControl_NeckRot = iVrpnConnector->getJointRotationByPosition(2, 1);

	/* Update left arm */
	SkelControl_LeftUpperarmRot = iVrpnConnector->getJointRotationByPosition(5, 6);
	SkelControl_LeftLowerarmRot = iVrpnConnector->getJointRotationByPosition(6, 7);
	SkelControl_LeftHandRot = iVrpnConnector->getJointRotationByPosition(7, 8);

	SkelControl_LeftHandTipRot = iVrpnConnector->getJointRotationByPosition(8, 9);
	SkelControl_LeftThumbRot = iVrpnConnector->getJointRotationByPosition(8, 9);

	/* Update right arm */
	SkelControl_RightUpperarmRot = iVrpnConnector->getJointRotationByPosition(11, 12);
	SkelControl_RightLowerarmRot = iVrpnConnector->getJointRotationByPosition(12, 13);
	SkelControl_RightHandRot = iVrpnConnector->getJointRotationByPosition(13, 14);

	SkelControl_RightHandTipRot = iVrpnConnector->getJointRotationByPosition(14, 15);
	SkelControl_RightThumbRot = iVrpnConnector->getJointRotationByPosition(14, 15);

	/* Update left leg */
	SkelControl_LeftThighRot = iVrpnConnector->getJointRotationByPosition(17, 16);
	SkelControl_LeftCalfRot = iVrpnConnector->getJointRotationByPosition(18, 17);
	SkelControl_LeftFootRot = iVrpnConnector->getJointRotationByPosition(19, 18);

	/* Update right leg */
	SkelControl_RightThighRot = iVrpnConnector->getJointRotationByPosition(21, 20);
	SkelControl_RightCalfRot = iVrpnConnector->getJointRotationByPosition(22, 21);
	SkelControl_RightFootRot = iVrpnConnector->getJointRotationByPosition(23, 22);
}

void UUnrealMeAnimInstance::updateRotationsByPositionsWithSmoothing()
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

FRotator UUnrealMeAnimInstance::getJointRotation(EJointRotationType aJoint)
{
	return iRotations[aJoint];
}