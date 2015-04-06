// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeVRPNConnector.h"

const char	*TRACKER_NAME = "Tracker0@localhost";
int	CONNECTION_PORT = vrpn_DEFAULT_LISTEN_PORT_NO;	// Port for connection to listen on

vrpn_Tracker_NULL	*cNullTracker;
vrpn_Tracker_Remote	*cTracker;
vrpn_Connection		*cConnection;
UnrealMeVRPNBone* cBones;
int cPosArrLen;
int cRotArrLen;

void VRPN_CALLBACK handle_pos(void *, const vrpn_TRACKERCB aTracker)
{
	const float tPosX = aTracker.pos[0];
	const float tPosY = aTracker.pos[1];
	const float tPosZ = aTracker.pos[2];

	FVector tCurrentPosition = UUnrealMeCoordinateHelper::convertPositionToUnrealSpace(tPosX, tPosY, tPosZ);
	cBones[aTracker.sensor].setCurrentPosition(tCurrentPosition);

	const float tQuatX = aTracker.quat[0];
	const float tQuatY = aTracker.quat[1];
	const float tQuatZ = aTracker.quat[2];
	const float tQuatW = aTracker.quat[3];

	FRotator tCurrentRotation = UUnrealMeCoordinateHelper::convertQuatRotationToRotator(UUnrealMeCoordinateHelper::convertQuatRotationToUnrealSpace(tQuatX, tQuatY, tQuatZ, tQuatW), 0);
	cBones[aTracker.sensor].setCurrentRotation(tCurrentRotation);
}

void VRPN_CALLBACK handle_vel(void *, const vrpn_TRACKERVELCB aTracker)
{
	cBones[aTracker.sensor].setCurrVel(*aTracker.vel);
}

void VRPN_CALLBACK handle_acc(void *, const vrpn_TRACKERACCCB aTracker)
{
	cBones[aTracker.sensor].setCurrAcc(*aTracker.acc);
}

void UUnrealMeVRPNConnector::initializeTracker()
{
	fprintf(stderr, "Tracker's name is %s.\n", TRACKER_NAME);
	cTracker = new vrpn_Tracker_Remote(TRACKER_NAME);
	printf("Tracker update: '.' = pos, '/' = vel, '~' = acc\n");
	cTracker->register_change_handler(NULL, handle_pos);
	cTracker->register_change_handler(NULL, handle_vel);
	cTracker->register_change_handler(NULL, handle_acc);
	cPosArrLen = 3;
	cRotArrLen = 4;
	cBones = new UnrealMeVRPNBone[24];

	for (int i = 0; i < 23; i++){
		//assigns pointers to the value arrays to each bone
		UnrealMeVRPNBone tTemp = UnrealMeVRPNBone(i);
		cBones[i] = tTemp;
	}
}

void UUnrealMeVRPNConnector::callTrackerMainloop()
{
	cTracker->mainloop();
	vrpn_SleepMsecs(1);
}

FVector UUnrealMeVRPNConnector::getBonePosition(int32 aBoneId)
{
	return cBones[aBoneId].getCurrentPosition();
}

FRotator UUnrealMeVRPNConnector::getBoneRotation(int32 aBoneId)
{
	return cBones[aBoneId].getCurrentRotation();
}

FQuat UUnrealMeVRPNConnector::getBoneQuaternion(int32 aBoneId)
{
	FQuat tBack;
	if (cBones[aBoneId].getCurrRot() == NULL){
		tBack.X = 0.0;
		tBack.Y = 0.0;
		tBack.Z = 0.0;
		tBack.W = 0.0;
	}
	else{
		tBack.X = cBones[aBoneId].getCurrRot()[0];
		tBack.Y = cBones[aBoneId].getCurrRot()[1];
		tBack.Z = cBones[aBoneId].getCurrRot()[2];
		tBack.W = cBones[aBoneId].getCurrRot()[3];
	}
	return tBack;
}

float UUnrealMeVRPNConnector::getBoneAcceleration(int32 aBoneId)
{
	return cBones[aBoneId].getCurrAcc();
}

float UUnrealMeVRPNConnector::getBoneVelocity(int32 aBoneId)
{
	return cBones[aBoneId].getCurrVel();
}

FRotator UUnrealMeVRPNConnector::getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint)
{
	FVector tStartJointPos;
	if (aStartJoint != 1)
	{
		tStartJointPos = cBones[aStartJoint].getCurrentPosition();
	}
	else
	{
		tStartJointPos = FVector(0, 0, 0);
	}

	FVector tEndJointPos;
	if (aEndJoint != 1)
	{
		tEndJointPos = cBones[aEndJoint].getCurrentPosition();
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