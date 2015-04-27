// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeVRPNConnector.h"

typedef struct _VRPN_CB_INFO
{
	int tTrackerRemoteIndex;
}VRPN_CB_INFO;

static int32 iTrackerRemoteCount;
static UnrealMeVRPNBone* iBones;

/* Initialize the singleton instance. */
UUnrealMeVRPNConnector* UUnrealMeVRPNConnector::iSingleton = NewObject<UUnrealMeVRPNConnector>();

UUnrealMeVRPNConnector::UUnrealMeVRPNConnector(const FObjectInitializer& PCIP) : Super(PCIP)
{
	
}

void VRPN_CALLBACK handle_pos(void* aParam, const vrpn_TRACKERCB aTracker)
{
	if (aParam != NULL)
	{
		int tIndex = ((VRPN_CB_INFO*)aParam)->tTrackerRemoteIndex;

		int tAccessIndex;

		if (iTrackerRemoteCount > 1)
		{
			tAccessIndex = tIndex;
		}
		else
		{
			tAccessIndex = aTracker.sensor;
		}

		if (iBones != NULL)
		{
			const float tPosX = aTracker.pos[0];
			const float tPosY = aTracker.pos[1];
			const float tPosZ = aTracker.pos[2];

			FVector tCurrentPosition = UUnrealMeCoordinateHelper::convertPositionToUnrealSpace(tPosX, tPosY, tPosZ);
			iBones[tAccessIndex].setCurrentPosition(tCurrentPosition);

			const float tQuatX = aTracker.quat[0];
			const float tQuatY = aTracker.quat[1];
			const float tQuatZ = aTracker.quat[2];
			const float tQuatW = aTracker.quat[3];

			FRotator tCurrentRotation = UUnrealMeCoordinateHelper::convertQuatRotationToRotator(UUnrealMeCoordinateHelper::convertQuatRotationToUnrealSpace(tQuatX, tQuatY, tQuatZ, tQuatW), 0);
			iBones[tAccessIndex].setCurrentRotation(tCurrentRotation);
		}
	}
}

void VRPN_CALLBACK handle_vel(void *, const vrpn_TRACKERVELCB aTracker)
{
	if (iBones != NULL)
	{
		iBones[aTracker.sensor].setCurrVel(*aTracker.vel);
	}
}

void VRPN_CALLBACK handle_acc(void *, const vrpn_TRACKERACCCB aTracker)
{
	if (iBones != NULL)
	{
		iBones[aTracker.sensor].setCurrAcc(*aTracker.acc);
	}
}

void UUnrealMeVRPNConnector::disconnect()
{
	if (iConnected)
	{
		iConnected = false;
		iTrackerRemoteCount = 0;

		for (int32 i = 0; i < iTrackerRemotes.Num(); i++)
		{
			iTrackerRemotes[i]->unregister_change_handler(NULL, handle_pos);
			iTrackerRemotes[i]->unregister_change_handler(NULL, handle_vel);
			iTrackerRemotes[i]->unregister_change_handler(NULL, handle_acc);
			delete iTrackerRemotes[i];
		}

		iTrackerRemotes.Empty();
	}
}

void UUnrealMeVRPNConnector::BeginDestroy()
{
	Super::BeginDestroy();
	disconnect();
	delete iBones;
	// Add more operations here if necessary
}

void UUnrealMeVRPNConnector::initializeConnection(TArray<FString> aTrackerNames, FString aServerAddress)
{
	if (iConnected == false)
	{
		iConnectionPort = vrpn_DEFAULT_LISTEN_PORT_NO;

		iPosArrLen = 3;
		iRotArrLen = 4;

		if (iBones == NULL)
		{
			iBones = new UnrealMeVRPNBone[24];

			for (int i = 0; i < 23; i++)
			{
				//assigns pointers to the value arrays to each bone
				UnrealMeVRPNBone tTemp = UnrealMeVRPNBone(i);
				iBones[i] = tTemp;
			}
		}

		FString tAt = FString(TEXT("@"));

		for (int32 i = 0; i < aTrackerNames.Num(); i++)
		{
			FString tCurrentTrackerName = aTrackerNames[i];

			FString tCurrentAddress = tCurrentTrackerName + tAt + aServerAddress;
			vrpn_Tracker_Remote* tCurrentTracker = new vrpn_Tracker_Remote(TCHAR_TO_ANSI(*tCurrentAddress));

			VRPN_CB_INFO* tCurrentInfo = new VRPN_CB_INFO();
			tCurrentInfo->tTrackerRemoteIndex = i;

			tCurrentTracker->register_change_handler(tCurrentInfo, handle_pos);
			tCurrentTracker->register_change_handler(tCurrentInfo, handle_vel);
			tCurrentTracker->register_change_handler(tCurrentInfo, handle_acc);

			iTrackerRemotes.Add(tCurrentTracker);
			iTrackerRemoteCount++;
		}

		iConnected = true;
	}
}

bool UUnrealMeVRPNConnector::isConnected()
{
	return iConnected;
}

void UUnrealMeVRPNConnector::callMainloop()
{
	for (int32 i = 0; i < iTrackerRemotes.Num(); i++)
	{
		if (iTrackerRemotes[i] != NULL)
		{
			iTrackerRemotes[i]->mainloop();
		}
	}

	vrpn_SleepMsecs(1);
}

FVector UUnrealMeVRPNConnector::getBonePosition(int32 aBoneId)
{
	if (iBones != NULL)
	{
		return iBones[aBoneId].getCurrentPosition();
	}
	else
	{
		return FVector::ZeroVector;
	}
}

FRotator UUnrealMeVRPNConnector::getBoneRotation(int32 aBoneId)
{
	if (iBones != NULL)
	{
		return iBones[aBoneId].getCurrentRotation();
	}
	else
	{
		return FRotator::ZeroRotator;
	}
}

FQuat UUnrealMeVRPNConnector::getBoneQuaternion(int32 aBoneId)
{
	FQuat tBack;
	if (iBones != NULL && iBones[aBoneId].getCurrRot() == NULL){
		tBack.X = 0.0;
		tBack.Y = 0.0;
		tBack.Z = 0.0;
		tBack.W = 0.0;
	}
	else
	{
		tBack.X = iBones[aBoneId].getCurrRot()[0];
		tBack.Y = iBones[aBoneId].getCurrRot()[1];
		tBack.Z = iBones[aBoneId].getCurrRot()[2];
		tBack.W = iBones[aBoneId].getCurrRot()[3];
	}
	return tBack;
}

float UUnrealMeVRPNConnector::getBoneAcceleration(int32 aBoneId)
{
	if (iBones != NULL)
	{
		return iBones[aBoneId].getCurrAcc();
	}
	else
	{
		return 0.0;
	}
}

float UUnrealMeVRPNConnector::getBoneVelocity(int32 aBoneId)
{
	if (iBones != NULL)
	{
		return iBones[aBoneId].getCurrVel();
	}
	else
	{
		return 0.0;
	}
}

FRotator UUnrealMeVRPNConnector::getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint)
{
	if (iBones != NULL)
	{
		FVector tStartJointPos;
		if (aStartJoint != 1)
		{
			tStartJointPos = iBones[aStartJoint].getCurrentPosition();
		}
		else
		{
			tStartJointPos = FVector(0, 0, 0);
		}

		FVector tEndJointPos;
		if (aEndJoint != 1)
		{
			tEndJointPos = iBones[aEndJoint].getCurrentPosition();
		}
		else
		{
			tEndJointPos = FVector(0, 0, 0);
		}

		FVector tRelative = FVector();
		tRelative.X = tStartJointPos.X - tEndJointPos.X;
		tRelative.Y = tStartJointPos.Y - tEndJointPos.Y;
		tRelative.Z = tStartJointPos.Z - tEndJointPos.Z;

		FRotator tBack = tRelative.Rotation();
		return tBack;
	}
	else
	{
		return FRotator::ZeroRotator;
	}
}

UUnrealMeVRPNConnector* UUnrealMeVRPNConnector::getInstance()
{
	return UUnrealMeVRPNConnector::iSingleton;
}