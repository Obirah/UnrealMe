// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UnrealMeVRPNConnector.h"
#include "UnrealMePortableActor.generated.h"

/**
 * Class for future use to implement a portable object.
 */
UCLASS()
class UNREALME_API AUnrealMePortableActor : public AActor
{
	GENERATED_BODY()
	
private:
	UUnrealMeVRPNConnector* iVrpnConnector;
public:	
	/** Sets default values for this actor's properties */
	AUnrealMePortableActor();

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	
	/** Called every frame */
	virtual void Tick( float DeltaSeconds ) override;	
};