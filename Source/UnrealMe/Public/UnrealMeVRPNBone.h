// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * Object containing position, rotation, velocity and acceleration data of a tracked joint.
 */
class UNREALME_API UnrealMeVRPNBone
{
private:
	int iId;
	float iCurrPos[3];
	float iCurrRot[4];
	float iCurrVelocity;
	float iCurrAcceleration;
	FVector iCurrentPosition;
	FRotator iCurrentRotation;
public:
	/** Non-parameterized constructor */
	UnrealMeVRPNBone();
	/** Constructor with a bone id */
	UnrealMeVRPNBone(int aBoneId);
	/** Constructor with a bone id and position and rotation arrays */
	UnrealMeVRPNBone(int aBoneId, const float* aPosArr, const float* aRotArr);
	/** Returns the id of a bone */
	int getId();
	/** Returns the position of a bone */
	const float* getCurrPos();
	/** Returns the rotation of a bone */
	const float* getCurrRot();
	/** Returns the veloctiy of a bone */
	const float getCurrVel();
	/** Returns the acceleration of a bone */
	const float getCurrAcc();
	/** Sets the position of a bone - array version */
	void setCurrPos(const float aPosArr[], const int aPosArrLen);
	/** Sets the position of a bone - float version => preferred */
	void setCurrPos(const float aX, const float aY, const float aZ);
	/** Sets the rotation of a bone - array version */
	void setCurrRot(const float* aRotArr, int aRotArrLen);
	/** Sets the position of a bone - float version => preferred */
	void setCurrRot(const float aQuatX, const float aQuatY, const float aQuatZ, const float aQuatW);
	/** Sets the velocity of a bone */
	void setCurrVel(const float aVelocity);
	/** Sets the acceleration of a bone */
	void setCurrAcc(const float aAcceleration);
	/** Sets the position of a bone - FVector version */
	void setCurrentPosition(FVector aPosition);
	/** Sets the rotation of a bone - FRotator version */
	void setCurrentRotation(FRotator aRotation);
	/** Returns the position of a bone - FVector version */
	FVector getCurrentPosition();
	/** Returns the rotation of a bone - FRotator version */
	FRotator getCurrentRotation();
	/** Default destructor */
	~UnrealMeVRPNBone();
};
