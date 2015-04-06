// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMeVRPNBone.h"

UnrealMeVRPNBone::UnrealMeVRPNBone()
{
}

UnrealMeVRPNBone::UnrealMeVRPNBone(int aBoneId){
	iId = aBoneId;
	for (int i = 0; i < 4; i++){
		if (i < 3){
			iCurrPos[i] = 0;
		}
		iCurrRot[i] = 0;
	}
}

UnrealMeVRPNBone::UnrealMeVRPNBone(int aBoneId, const float* aPosArr, const float* aRotArr){
	iId = aBoneId;

	for (int i = 0; i < 4; i++){
		if (i < 4){
			iCurrPos[i] = aPosArr[i];
		}
		iCurrRot[i] = aRotArr[i];
	}
}

int UnrealMeVRPNBone::getId(){
	return iId;
}

const float* UnrealMeVRPNBone::getCurrPos(){
	return iCurrPos;
}

const float* UnrealMeVRPNBone::getCurrRot(){
	return iCurrRot;
}

const float UnrealMeVRPNBone::getCurrVel(){
	return iCurrVelocity;
}

const float UnrealMeVRPNBone::getCurrAcc(){
	return iCurrAcceleration;
}

void UnrealMeVRPNBone::setCurrPos(const float aPosArr[], const int aPosArrLen){
	for (int i = 0; i < 3; i++){
		iCurrPos[i] = aPosArr[i];
	}
}

void UnrealMeVRPNBone::setCurrPos(const float aX, const float aY, const float aZ){
	iCurrPos[0] = aX;
	iCurrPos[1] = aY;
	iCurrPos[2] = aZ;
}

void UnrealMeVRPNBone::setCurrRot(const float* aRotArr, int aRotArrLen){
	for (int i = 0; i < 4; i++){
		iCurrRot[i] = aRotArr[i];
	}
}

void UnrealMeVRPNBone::setCurrRot(const float aQuatX, const float aQuatY, const float aQuatZ, const float aQuatW){
	iCurrRot[0] = aQuatX;
	iCurrRot[1] = aQuatY;
	iCurrRot[2] = aQuatZ;
	iCurrRot[3] = aQuatW;
}

void UnrealMeVRPNBone::setCurrVel(const float aVelocity){
	iCurrVelocity = aVelocity;
}

void UnrealMeVRPNBone::setCurrAcc(const float aAcceleration){
	iCurrAcceleration = aAcceleration;
}

void UnrealMeVRPNBone::setCurrentPosition(FVector aPosition)
{
	iCurrentPosition = aPosition;
}

void UnrealMeVRPNBone::setCurrentRotation(FRotator aRotation)
{
	iCurrentRotation = aRotation;
}

FVector UnrealMeVRPNBone::getCurrentPosition()
{
	return iCurrentPosition;
}

FRotator UnrealMeVRPNBone::getCurrentRotation()
{
	return iCurrentRotation;
}

UnrealMeVRPNBone::~UnrealMeVRPNBone()
{
}
