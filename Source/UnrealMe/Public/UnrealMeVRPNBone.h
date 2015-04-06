// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
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
	UnrealMeVRPNBone();
	UnrealMeVRPNBone(int aBoneId);
	UnrealMeVRPNBone(int aBoneId, const float* aPosArr, const float* aRotArr);
	int getId();
	const float* getCurrPos();
	const float* getCurrRot();
	const float getCurrVel();
	const float getCurrAcc();
	void setCurrPos(const float aPosArr[], const int aPosArrLen);
	void setCurrPos(const float aX, const float aY, const float aZ);
	void setCurrRot(const float* aRotArr, int aRotArrLen);
	void setCurrRot(const float aQuatX, const float aQuatY, const float aQuatZ, const float aQuatW);
	void setCurrVel(const float aVelocity);
	void setCurrAcc(const float aAcceleration);
	void setCurrentPosition(FVector aPosition);
	void setCurrentRotation(FRotator aRotation);
	FVector getCurrentPosition();
	FRotator getCurrentRotation();
	~UnrealMeVRPNBone();
};
