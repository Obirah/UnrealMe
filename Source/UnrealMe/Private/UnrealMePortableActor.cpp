// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealMe.h"
#include "UnrealMePortableActor.h"


// Sets default values
AUnrealMePortableActor::AUnrealMePortableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnrealMePortableActor::BeginPlay()
{
	Super::BeginPlay();
	iVrpnConnector = UUnrealMeVRPNConnector::getInstance();
}

// Called every frame
void AUnrealMePortableActor::Tick(float aDeltaTime)
{
	Super::Tick(aDeltaTime);
	
}

