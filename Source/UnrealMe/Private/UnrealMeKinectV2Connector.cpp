// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeKinectV2Connector.h"

static TStaticArray<std::map<int, FVector>, 6> iUsersSkeletonData;
static std::map<int, FVector> iSkeletonData;
static std::map<int, bool> iUserTrackingState;
static std::map<int, FString> iJointToSkeletalBone;

static CameraSpacePoint iPreviousTorsoPos = CameraSpacePoint();
static FVector iCurrentTorsoDelta = FVector(0, 0, 0);

static int iTrackedUsers = 0;

static IKinectSensor* iKinectSensor;
static ICoordinateMapper* iCoordinateMapper;
static IBodyFrameReader* iBodyFrameReader;

CameraSpacePoint getRelativePosition(CameraSpacePoint aTorso, CameraSpacePoint aOtherJoint)
{
	CameraSpacePoint tBack = CameraSpacePoint();

	tBack.X = aTorso.X - aOtherJoint.X;
	tBack.Y = aTorso.Y - aOtherJoint.Y;
	tBack.Z = aTorso.Z - aOtherJoint.Z;

	return tBack;
}

FVector convertToUnrealSpace(CameraSpacePoint aPosition)
{
	return FVector(aPosition.Z * 100, aPosition.X * -100, aPosition.Y * -100);
}

void checkDeltaForOutliers(int32 aThreshold)
{
	if (iCurrentTorsoDelta.X > aThreshold || iCurrentTorsoDelta.Y > aThreshold || iCurrentTorsoDelta.Z > aThreshold)
	{
		iCurrentTorsoDelta = FVector(0, 0, 0);
	}
}

UUnrealMeKinectV2Connector::UUnrealMeKinectV2Connector(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	
}

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
void UUnrealMeKinectV2Connector::initializeKinect()
{
	HRESULT hr;
	hr = GetDefaultKinectSensor(&iKinectSensor);

	if (iKinectSensor)
	{
		IBodyFrameSource* tBodyFrameSource = NULL;

		hr = iKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = iKinectSensor->get_CoordinateMapper(&iCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			UE_LOG(UnrealMeInit, Log, TEXT("Coordinate mapper successfully initialized."));
			hr = iKinectSensor->get_BodyFrameSource(&tBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			UE_LOG(UnrealMeInit, Log, TEXT("Body frame source successfully initialized."));
			hr = tBodyFrameSource->OpenReader(&iBodyFrameReader);
			/* If the initialization worke so far, we can initialize one of the maps we'll need later on. */
			initializeTrackingStateMap();
			initializeJointToSkeletalBoneMapping();
		}

		SafeRelease(tBodyFrameSource);
		UE_LOG(UnrealMeInit, Log, TEXT("End of initialization sequence."));
	}
}

void UUnrealMeKinectV2Connector::processBody(INT64 aTime, int aBodyCount, IBody** aBodies)
{
	HRESULT hr;

	if (iCoordinateMapper)
	{
		TStaticArray<std::map<int, FVector>, 6> tUsersSkeletonData;
		int tTrackedUsers = 0;

		for (int i = 0; i < aBodyCount; ++i)
		{
			//tTrackedUsers++;
			IBody* tBody = aBodies[i];

			if (tBody)
			{
				BOOLEAN tIsBodyTracked = false;
				hr = tBody->get_IsTracked(&tIsBodyTracked);

				if (SUCCEEDED(hr) && tIsBodyTracked)
				{
					iUserTrackingState[i] = true;
					Joint tJoints[JointType_Count];
					hr = tBody->GetJoints(_countof(tJoints), tJoints);

					if (SUCCEEDED(hr))
					{
						CameraSpacePoint tTorsoPosition;
						bool tTorsoInitialized = false;
						std::map<int, FVector> tSkeletonData;						

						for (int j = 0; j < _countof(tJoints); ++j)
						{
							CameraSpacePoint tPosition = tJoints[j].Position;
							JointType tJointType = tJoints[j].JointType;

							if (j == 0)
							{
								tSkeletonData[j] = convertToUnrealSpace(tPosition);
							}

							if (j == 1)
							{
								tSkeletonData[j] = convertToUnrealSpace(tPosition);
								tTorsoPosition = tPosition;
								tTorsoInitialized = true;

								if (iPreviousTorsoPos.X != 0 && iPreviousTorsoPos.Y != 0 && iPreviousTorsoPos.Z != 0)
								{
									CameraSpacePoint tTempDelta = getRelativePosition(iPreviousTorsoPos, tTorsoPosition);
									iCurrentTorsoDelta = convertToUnrealSpace(tTempDelta);
									checkDeltaForOutliers(10);
								}

								iPreviousTorsoPos = tTorsoPosition;

								tPosition.X = tSkeletonData[j - 1].X;
								tPosition.Y = tSkeletonData[j - 1].Y;
								tPosition.Z = tSkeletonData[j - 1].Z;
								tPosition = getRelativePosition(tTorsoPosition, tPosition);
								tSkeletonData[j-1] = FVector(tPosition.X, tPosition.Y, tPosition.Z);
							}

							if (tTorsoInitialized && j > 1)
							{
								tPosition = getRelativePosition(tTorsoPosition, tPosition);
								tSkeletonData[j] = convertToUnrealSpace(tPosition);

								/*stringstream tStringStream;
								tStringStream << tJointType << ": X=" << tPosition.X << " Y=" << tPosition.Y << " Z=" << tPosition.Z << endl;
								cout << tStringStream.str() << endl;*/

							}
						}

						iSkeletonData = tSkeletonData;
						tUsersSkeletonData[i] = tSkeletonData;
					}
				}

			}
		}

		updateData(tUsersSkeletonData);
		
		iTrackedUsers = tTrackedUsers;
	}
}

void UUnrealMeKinectV2Connector::updateData(TStaticArray<std::map<int, FVector>, 6> aData)
{
	for (int i = 0; i < 6; i++)
	{
		iUsersSkeletonData[i] = aData[i];
	}
}

void UUnrealMeKinectV2Connector::update()
{
	if (!iBodyFrameReader)
	{
		UE_LOG(UnrealMeInit, Log, TEXT("No body frame reader available."));
	}

	IBodyFrame* tBodyFrame = NULL;
	HRESULT hr = iBodyFrameReader->AcquireLatestFrame(&tBodyFrame);

	if (SUCCEEDED(hr))
	{
		INT64 tTime = 0;
		hr = tBodyFrame->get_RelativeTime(&tTime);

		IBody* tBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(hr))
		{
			hr = tBodyFrame->GetAndRefreshBodyData(_countof(tBodies), tBodies);
		}

		if (SUCCEEDED(hr))
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

FVector UUnrealMeKinectV2Connector::getJointPosition(int32 aJointId)
{
	return iSkeletonData[aJointId];
}

FVector UUnrealMeKinectV2Connector::getUserJointPosition(int32 aUserId, int32 aJointId)
{
	std::map<int, FVector> tTargetUserSkeletonData = iUsersSkeletonData[aUserId];
	return tTargetUserSkeletonData[aJointId];
}

int32 UUnrealMeKinectV2Connector::getTrackedUsersCount()
{
	return iTrackedUsers;
}

FVector UUnrealMeKinectV2Connector::getCurrentTorsoDelta()
{
	return iCurrentTorsoDelta;
}

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

FString UUnrealMeKinectV2Connector::getBoneNameByJoint(int32 aJointId)
{
	return iJointToSkeletalBone[aJointId];
}

void UUnrealMeKinectV2Connector::disconnectKinect()
{
	SafeRelease(iBodyFrameReader);
	SafeRelease(iCoordinateMapper);
	iKinectSensor->Close();
	SafeRelease(iKinectSensor);
}