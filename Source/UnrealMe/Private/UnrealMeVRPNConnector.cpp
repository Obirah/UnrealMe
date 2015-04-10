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
	iConnectionPort = vrpn_DEFAULT_LISTEN_PORT_NO;

	iPosArrLen = 3;
	iRotArrLen = 4;

	iBones = new UnrealMeVRPNBone[24];

	for (int i = 0; i < 23; i++)
	{
		//assigns pointers to the value arrays to each bone
		UnrealMeVRPNBone tTemp = UnrealMeVRPNBone(i);
		iBones[i] = tTemp;
	}
}

void VRPN_CALLBACK handle_pos(void* aParam, const vrpn_TRACKERCB aTracker)
{
	int tIndex = ((VRPN_CB_INFO*)aParam)->tTrackerRemoteIndex;
	
	//int tControl = 99;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Callback exploration - Control value: %d, Index from param: %d"), tControl, tIndex));

	int tAccessIndex;

	if (iTrackerRemoteCount > 1)
	{
		tAccessIndex = tIndex;
	}
	else
	{
		tAccessIndex = aTracker.sensor;
	}

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

void VRPN_CALLBACK handle_vel(void *, const vrpn_TRACKERVELCB aTracker)
{
	iBones[aTracker.sensor].setCurrVel(*aTracker.vel);
}

void VRPN_CALLBACK handle_acc(void *, const vrpn_TRACKERACCCB aTracker)
{
	iBones[aTracker.sensor].setCurrAcc(*aTracker.acc);
}

void UUnrealMeVRPNConnector::BeginDestroy()
{
	Super::BeginDestroy();

	for (int32 i = 0; i < iTrackerRemotes.Num(); i++)
	{
		iTrackerRemotes[i]->unregister_change_handler(NULL, handle_pos);
		iTrackerRemotes[i]->unregister_change_handler(NULL, handle_vel);
		iTrackerRemotes[i]->unregister_change_handler(NULL, handle_acc);
	}

	iTrackerRemotes.Empty();
	delete iBones;
}

void UUnrealMeVRPNConnector::initializeConnection(TArray<FString> aTrackerNames, FString aServerAddress)
{
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

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("VRPN connection initizalized: %s, Trackers: %d"), *tCurrentAddress, iTrackerRemoteCount));
	}	

	iConnected = true;
}

bool UUnrealMeVRPNConnector::isConnected()
{
	return iConnected;
}

void UUnrealMeVRPNConnector::callMainloop()
{
	for (int32 i = 0; i < iTrackerRemotes.Num(); i++)
	{
		iTrackerRemotes[i]->mainloop();
	}

	vrpn_SleepMsecs(1);
}

FVector UUnrealMeVRPNConnector::getBonePosition(int32 aBoneId)
{
	return iBones[aBoneId].getCurrentPosition();
}

FRotator UUnrealMeVRPNConnector::getBoneRotation(int32 aBoneId)
{
	return iBones[aBoneId].getCurrentRotation();
}

FQuat UUnrealMeVRPNConnector::getBoneQuaternion(int32 aBoneId)
{
	FQuat tBack;
	if (iBones[aBoneId].getCurrRot() == NULL){
		tBack.X = 0.0;
		tBack.Y = 0.0;
		tBack.Z = 0.0;
		tBack.W = 0.0;
	}
	else{
		tBack.X = iBones[aBoneId].getCurrRot()[0];
		tBack.Y = iBones[aBoneId].getCurrRot()[1];
		tBack.Z = iBones[aBoneId].getCurrRot()[2];
		tBack.W = iBones[aBoneId].getCurrRot()[3];
	}
	return tBack;
}

float UUnrealMeVRPNConnector::getBoneAcceleration(int32 aBoneId)
{
	return iBones[aBoneId].getCurrAcc();
}

float UUnrealMeVRPNConnector::getBoneVelocity(int32 aBoneId)
{
	return iBones[aBoneId].getCurrVel();
}

FRotator UUnrealMeVRPNConnector::getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint)
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

	/*FQuat tQuaternion = getQuaternionFromVectors(tStartJointPos, tEndJointPos);*/
	//tQuaternion.Rotator();

	FRotator tBack = tRelative.Rotation();
	return tBack;
}

UUnrealMeVRPNConnector* UUnrealMeVRPNConnector::getInstance()
{
	return UUnrealMeVRPNConnector::iSingleton;
}