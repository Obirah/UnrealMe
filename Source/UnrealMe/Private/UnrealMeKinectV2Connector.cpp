// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeKinectV2Connector.h"

/* COLLECTIONS */
static TStaticArray<std::map<int, FVector>, 6> iUsersSkeletonData;
static std::map<int, FVector> iSkeletonData;
static std::map<int, bool> iUserTrackingState;
static std::map<int, FString> iJointToSkeletalBone;

static std::map<int, FRotator> iSkeletonRotationData;

static TStaticArray<FRotations, 25> iBuffer;

/* TORSO POSITIONS AT TIME T-1 */
static CameraSpacePoint iPreviousTorsoPos;
static std::map<int, CameraSpacePoint> iUsersPreviousTorsoPos;
/* TORSO POSITIONS AT TIME T */
static FVector iCurrentTorsoDelta;
static std::map<int, FVector> iUsersTorsoDeltas;

/* KINECT VARIABLES */
static IKinectSensor* iKinectSensor;
static ICoordinateMapper* iCoordinateMapper;
static IBodyFrameReader* iBodyFrameReader;

/* MISCELLANEOUS */
static int iTrackedUsers;
static bool iMultiUser;

/* TEST */
static int iRotationBufferSize;

static int iRotationXOffset = 0;
static int iRotationYOffset = 0;
static int iRotationZOffset = 0;

/*
 * HELPER FUNCTIONS (don't belong to the actual class)
 */

/* Convert absolute position to relative position */
CameraSpacePoint getRelativePosition(CameraSpacePoint aTorso, CameraSpacePoint aOtherJoint)
{
	CameraSpacePoint tBack = CameraSpacePoint();

	tBack.X = aTorso.X - aOtherJoint.X;
	tBack.Y = aTorso.Y - aOtherJoint.Y;
	tBack.Z = aTorso.Z - aOtherJoint.Z;

	return tBack;
}

/* 
 * Conversion from Kinect to Unreal coordinate system:
 * Kinect X	=> Unreal -Y
 * Kinect Y => Unreal -Z
 * Kinect Z => Unreal X
 */
FVector convertPositionToUnrealSpace(CameraSpacePoint aPosition)
{
	return FVector(aPosition.Z * 100, aPosition.X * -100, aPosition.Y * -100);
}

FQuat convertRotationToUnrealSpace(Vector4 aQuaternion)
{
	return FQuat(aQuaternion.z, aQuaternion.x, aQuaternion.y, aQuaternion.w);
}

/* DEFAULT CONSTRUCTOR (doing variable initializations) */
UUnrealMeKinectV2Connector::UUnrealMeKinectV2Connector(const FObjectInitializer& PCIP) : Super(PCIP)
{

}

/*
 * KINECT CONTROL FUNCTIONS FOR THE APPLICATION
 */

/* Connect the Kinect. */
void UUnrealMeKinectV2Connector::initializeKinect(bool aMultiUser)
{
	iMultiUser = aMultiUser;

	/* Variable default initializations */
	iPreviousTorsoPos = CameraSpacePoint();
	iCurrentTorsoDelta = FVector(0, 0, 0);
	iTrackedUsers = 0;
	iMultiUser = false;
	iRotationBufferSize = 0;

	/*iBuffer = TStaticArray<FRotations, 24>();*/
	for (int i = 0; i < 25; i++)
	{
		iBuffer[i] = FRotations();
	}

	HRESULT tCurrentOperation;
	tCurrentOperation = GetDefaultKinectSensor(&iKinectSensor);

	if (iKinectSensor)
	{
		IBodyFrameSource* tBodyFrameSource = NULL;

		tCurrentOperation = iKinectSensor->Open();

		/* Subsequently go through the connection process and check each step. */
		if (SUCCEEDED(tCurrentOperation))
		{
			tCurrentOperation = iKinectSensor->get_CoordinateMapper(&iCoordinateMapper);
		}

		if (SUCCEEDED(tCurrentOperation))
		{
			UE_LOG(UnrealMeInit, Log, TEXT("Coordinate mapper successfully initialized."));
			tCurrentOperation = iKinectSensor->get_BodyFrameSource(&tBodyFrameSource);
		}

		if (SUCCEEDED(tCurrentOperation))
		{
			UE_LOG(UnrealMeInit, Log, TEXT("Body frame source successfully initialized."));
			tCurrentOperation = tBodyFrameSource->OpenReader(&iBodyFrameReader);
			/* If the initialization worke so far, we can initialize one of the maps we'll need later on. */
			initializeTrackingStateMap();
			initializeJointToSkeletalBoneMapping();
		}

		/* The body frame source can be dereferenced after the Body Frame Reader is initialized. */
		SafeRelease(tBodyFrameSource);
		UE_LOG(UnrealMeInit, Log, TEXT("End of initialization sequence."));
	}
}

/* High-level update method => does all neccessary internal logic and is used to be called from Unreal's Tick event. */
void UUnrealMeKinectV2Connector::update()
{
	if (!iBodyFrameReader)
	{
		UE_LOG(UnrealMeInit, Log, TEXT("No body frame reader available."));
	}

	IBodyFrame* tBodyFrame = NULL;
	HRESULT tCurrentOperation = iBodyFrameReader->AcquireLatestFrame(&tBodyFrame);

	if (SUCCEEDED(tCurrentOperation))
	{
		INT64 tTime = 0;
		tCurrentOperation = tBodyFrame->get_RelativeTime(&tTime);

		IBody* tBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(tCurrentOperation))
		{
			tCurrentOperation = tBodyFrame->GetAndRefreshBodyData(_countof(tBodies), tBodies);
		}

		if (SUCCEEDED(tCurrentOperation))
		{
			processBody(tTime, _countof(tBodies), tBodies);
		}

		for (int i = 0; i < _countof(tBodies); ++i)
		{
			SafeRelease(tBodies[i]);
		}
	}

	SafeRelease(tBodyFrame);
}

/* Disconnect the Kinect. */
void UUnrealMeKinectV2Connector::disconnectKinect()
{
	iRotationXOffset = 0;
	iRotationYOffset = 0;
	iRotationZOffset = 0;

	SafeRelease(iBodyFrameReader);
	SafeRelease(iCoordinateMapper);
	iKinectSensor->Close();
	SafeRelease(iKinectSensor);
}

/* 
 * INTERNAL PROCESSING LOGIC
 */

void UUnrealMeKinectV2Connector::initializeTrackingStateMap()
{
	for (int i = 0; i < 6; i++)
	{
		iUserTrackingState[i] = false;
	}
}

void UUnrealMeKinectV2Connector::initializeJointToSkeletalBoneMapping()
{
	iJointToSkeletalBone[0] = "pelvis";
	iJointToSkeletalBone[1] = "spine_02";
	iJointToSkeletalBone[2] = "neck_01";
	iJointToSkeletalBone[3] = "head";
	iJointToSkeletalBone[4] = "upperarm_l";
	iJointToSkeletalBone[5] = "lowerarm_l";
	iJointToSkeletalBone[6] = "";
	iJointToSkeletalBone[7] = "hand_l";
	iJointToSkeletalBone[8] = "upperarm_r";
	iJointToSkeletalBone[9] = "lowerarm_r";
	iJointToSkeletalBone[10] = "";
	iJointToSkeletalBone[11] = "hand_r";
	iJointToSkeletalBone[12] = "thigh_l";
	iJointToSkeletalBone[13] = "calf_l";
	iJointToSkeletalBone[14] = "foot_l";
	iJointToSkeletalBone[15] = "ball_l";
	iJointToSkeletalBone[16] = "thigh_r";
	iJointToSkeletalBone[17] = "calf_r";
	iJointToSkeletalBone[18] = "foot_r";
	iJointToSkeletalBone[19] = "ball_r";
	iJointToSkeletalBone[20] = "spine_03";
	iJointToSkeletalBone[21] = "index_03_l";
	iJointToSkeletalBone[22] = "thumb_03_l";
	iJointToSkeletalBone[23] = "index_03_r";
	iJointToSkeletalBone[24] = "thumb_03_r";

	/* THE KINECT JOINTS AND IDS AS A REFERENCE:

	JointType_SpineBase	= 0,
	JointType_SpineMid	= 1,
	JointType_Neck	= 2,
	JointType_Head	= 3,
	JointType_ShoulderLeft	= 4,
	JointType_ElbowLeft	= 5,
	JointType_WristLeft	= 6,
	JointType_HandLeft	= 7,
	JointType_ShoulderRight	= 8,
	JointType_ElbowRight	= 9,
	JointType_WristRight	= 10,
	JointType_HandRight	= 11,
	JointType_HipLeft	= 12,
	JointType_KneeLeft	= 13,
	JointType_AnkleLeft	= 14,
	JointType_FootLeft	= 15,
	JointType_HipRight	= 16,
	JointType_KneeRight	= 17,
	JointType_AnkleRight	= 18,
	JointType_FootRight	= 19,
	JointType_SpineShoulder	= 20,
	JointType_HandTipLeft	= 21,
	JointType_ThumbLeft	= 22,
	JointType_HandTipRight	= 23,
	JointType_ThumbRight	= 24,

	*/
}

/* Checks if the passed torso delta is an outlier (which is defined by the passed threshold). */
FVector UUnrealMeKinectV2Connector::checkDeltaForOutliers(int32 aThreshold, FVector aDelta)
{
	/* If any of the dimensions is above the threshold the delta is discarded. */
	if (iCurrentTorsoDelta.X > aThreshold || iCurrentTorsoDelta.Y > aThreshold || iCurrentTorsoDelta.Z > aThreshold)
	{
		return FVector(0, 0, 0);
	}
	else
	{
		return aDelta;
	}
}

/* Update loops for the Kinect data. */
void UUnrealMeKinectV2Connector::processBody(INT64 aTime, int aBodyCount, IBody** aBodies)
{
	HRESULT tCurrentOperation;

	if (iCoordinateMapper)
	{
		/* Use temporary collections, not the global ones => prevention of concurrent modification exceptions. */
		TStaticArray<std::map<int, FVector>, 6> tUsersSkeletonData;
		TStaticArray<FVector, 6> tUsersTorsoDeltas;
		TStaticArray<CameraSpacePoint, 6> tUsersPreviousTorsoPosition;

		int tTrackedUsers = 0;

		/* Iterate over all bodies that theoretically may be tracked. */
		for (int i = 0; i < aBodyCount; ++i)
		{
			IBody* tBody = aBodies[i];

			if (tBody)
			{
				BOOLEAN tIsBodyTracked = false;
				tCurrentOperation = tBody->get_IsTracked(&tIsBodyTracked);

				if (SUCCEEDED(tCurrentOperation) && tIsBodyTracked)
				{
					iUserTrackingState[i] = true;

					Joint tJoints[JointType_Count];
					JointOrientation tJointOrientations[JointType_Count];

					tCurrentOperation = tBody->GetJoints(_countof(tJoints), tJoints);
					HRESULT tCurrentOperation2 = tBody->GetJointOrientations(_countof(tJointOrientations), tJointOrientations);

					if (SUCCEEDED(tCurrentOperation) && SUCCEEDED(tCurrentOperation2))
					{
						tTrackedUsers++;

						CameraSpacePoint tTorsoPosition = CameraSpacePoint();
						bool tTorsoInitialized = false;
						std::map<int, FVector> tSkeletonData;
						std::map<int, FRotator> tSkeletonRotationData;

						/* Iterate over all the joints of the current body's skeleton. */
						for (int j = 0; j < _countof(tJoints); ++j)
						{
							CameraSpacePoint tPosition = tJoints[j].Position;
							JointType tJointType = tJoints[j].JointType;

							Vector4 tJointOrientation = tJointOrientations[j].Orientation;

							/* Just convert and save the first spine joint (represented by index 0), it will be related to the torso later. */
							if (j == 0)
							{
								tSkeletonData[j] = convertPositionToUnrealSpace(tPosition);								
							}

							/* Index 1 represents the torso joint that we use as a reference for all the other joints later. */
							if (j == 1)
							{
								tSkeletonData[j] = convertPositionToUnrealSpace(tPosition);
								tTorsoPosition = tPosition;
								tTorsoInitialized = true;

								/* Save the delta of the torso between the current and the previous update iteration. */
								if (!iMultiUser)
								{
									if (iPreviousTorsoPos.X != 0 && iPreviousTorsoPos.Y != 0 && iPreviousTorsoPos.Z != 0)
									{
										CameraSpacePoint tTempDelta = getRelativePosition(iPreviousTorsoPos, tTorsoPosition);
										iCurrentTorsoDelta = checkDeltaForOutliers(10, convertPositionToUnrealSpace(tTempDelta));
										
									}

									iPreviousTorsoPos = tTorsoPosition;
								}
								else
								{
									if (iUsersPreviousTorsoPos[i].X != 0 && iUsersPreviousTorsoPos[i].Y != 0 && iUsersPreviousTorsoPos[i].Z != 0)
									{
										CameraSpacePoint tTempDelta = getRelativePosition(iPreviousTorsoPos, tTorsoPosition);
										tUsersTorsoDeltas[i] = checkDeltaForOutliers(10, convertPositionToUnrealSpace(tTempDelta));
									}

									iUsersPreviousTorsoPos[i] = tTorsoPosition;
								}														

								/* Now that we have saved the torso joint we can relate the spine joint of index 0 to it. */
								tPosition.X = tSkeletonData[j - 1].X;
								tPosition.Y = tSkeletonData[j - 1].Y;
								tPosition.Z = tSkeletonData[j - 1].Z;

								tPosition = getRelativePosition(tTorsoPosition, tPosition);

								tSkeletonData[j-1] = FVector(tPosition.X, tPosition.Y, tPosition.Z);
							}

							/* If the torso's initialized we can update all other joints. */
							if (tTorsoInitialized && j > 1)
							{
								tPosition = getRelativePosition(tTorsoPosition, tPosition);
								tSkeletonData[j] = convertPositionToUnrealSpace(tPosition);								
							}

							FQuat tQuaternionRotation = FQuat(tJointOrientation.x, tJointOrientation.y, tJointOrientation.z, tJointOrientation.w);//convertRotationToUnrealSpace(tJointOrientation);
							FRotator tKinectRotator = tQuaternionRotation.Rotator();

							FRotator tUnrealRotator = FRotator();
							tUnrealRotator.Roll = tKinectRotator.Yaw;
							tUnrealRotator.Pitch = tKinectRotator.Pitch;
							tUnrealRotator.Yaw = tKinectRotator.Roll;

							tSkeletonRotationData[j] = tUnrealRotator;							
							
							//iBuffer[j].addRotatorAt(iRotationBufferSize, tKinectRotator);
						}

						/* Update the global collections. */
						iSkeletonData = tSkeletonData;
						iSkeletonRotationData = tSkeletonRotationData;
						//iSkeletonRotationBuffer[iRotationBufferSize] = tSkeletonRotationData;
						iRotationBufferSize++;

						/*if (iRotationBufferSize == 9)
						{
							for (int i = 0; i < 25; i++)
							{
								FRotator tKinectRotator = iBuffer[i].getMeanRotator();

								FRotator tUnrealRotator = FRotator();
								tUnrealRotator.Roll = tKinectRotator.Yaw;
								tUnrealRotator.Pitch = tKinectRotator.Pitch;
								tUnrealRotator.Yaw = tKinectRotator.Roll;

								tSkeletonRotationData[i] = tUnrealRotator;
							}

							iSkeletonRotationData = tSkeletonRotationData;
							iRotationBufferSize = 0;
						}*/

						/* Update the value of the current key value pair in the multi user map if neccessary. */
						if (iMultiUser)
						{
							tUsersSkeletonData[i] = tSkeletonData;
						}						
					}
				}

			}
		}

		/* Do additional updates of global collections if  */
		if (iMultiUser)
		{
			updateData(tUsersSkeletonData, tUsersTorsoDeltas, tUsersPreviousTorsoPosition);
		}
		iTrackedUsers = tTrackedUsers;
	}
}

/* Helper method to update the global collections for multi user tracking. */
void UUnrealMeKinectV2Connector::updateData(TStaticArray<std::map<int, FVector>, 6> aPositions, TStaticArray<FVector, 6> aDeltas, TStaticArray<CameraSpacePoint, 6> aPreviousTorsoPositions)
{
	for (int i = 0; i < 6; i++)
	{
		iUsersSkeletonData[i] = aPositions[i];
		iUsersTorsoDeltas[i] = aDeltas[i];
		iUsersPreviousTorsoPos[i] = aPreviousTorsoPositions[i];
	}
}

/*
 * FUNCTIONS FOR SINGLE USER TRACKING
 */

/* Get the joint position corresponding to the passed id. */
FVector UUnrealMeKinectV2Connector::getJointPosition(int32 aJointId)
{
	return iSkeletonData[aJointId];
}

/* Get the current torso delta. */
FVector UUnrealMeKinectV2Connector::getCurrentTorsoDelta()
{
	return iCurrentTorsoDelta;
}

/** Get the joint orientation corresponding to the passed id */
FRotator UUnrealMeKinectV2Connector::getJointRotation(int32 aJointId)
{
	FRotator tBack = FRotator();
	tBack.Roll = iSkeletonRotationData[aJointId].Roll + iRotationXOffset;
	tBack.Pitch = iSkeletonRotationData[aJointId].Pitch + iRotationYOffset;
	tBack.Yaw = iSkeletonRotationData[aJointId].Yaw + iRotationZOffset;
	return tBack;
}

/** Derive the joint rotation from the relative position of one joint to another */
FRotator UUnrealMeKinectV2Connector::getJointRotationByPosition(int32 aStartJoint, int32 aEndJoint)
{
	FVector tStartJointPos;
	if (aStartJoint != 1)
	{
		tStartJointPos = iSkeletonData[aStartJoint];
	}
	else
	{
		tStartJointPos = FVector(0, 0, 0);
	}

	FVector tEndJointPos;
	if (aEndJoint != 1)
	{
		tEndJointPos = iSkeletonData[aEndJoint];
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

/*
* FUNCTIONS FOR MULTI USER TRACKING
*/

/** Get the joint position corresponding to the passed joint id and user id. */
FVector UUnrealMeKinectV2Connector::getUserJointPosition(int32 aUserId, int32 aJointId)
{
	std::map<int, FVector> tTargetUserSkeletonData = iUsersSkeletonData[aUserId];
	return tTargetUserSkeletonData[aJointId];
}

/** Get the amount of currently tracked user. */
int32 UUnrealMeKinectV2Connector::getTrackedUsersCount()
{
	return iTrackedUsers;
}

/* Check if the user with the given id is tracked. */
bool UUnrealMeKinectV2Connector::isUserTracked(int32 aUserId)
{
	bool tBack;

	/* If the passed user id is smaller than 0 or bigger than 6, the parameter is invalid and we need to avoid an out of bounds exception. */
	if (aUserId > 6 || aUserId < 0)
	{
		tBack = false;
	}
	else
	{
		tBack = iUserTrackingState[aUserId];
	}

	return tBack;
}

/* Get the bone name corresponding to the passed id. */
FString UUnrealMeKinectV2Connector::getBoneNameByJoint(int32 aJointId)
{
	return iJointToSkeletalBone[aJointId];
}

int32 UUnrealMeKinectV2Connector::offsetRotation(int32 aAxis, int32 aDegrees)
{
	int32 tBack = 0;

	switch (aAxis)
	{
	case 1:
		iRotationXOffset += aDegrees;
		tBack = iRotationXOffset;
		break;
	case 2:
		iRotationYOffset += aDegrees;
		tBack = iRotationYOffset;
		break;
	case 3:
		iRotationZOffset += aDegrees;
		tBack = iRotationZOffset;
    }

	return tBack;
}