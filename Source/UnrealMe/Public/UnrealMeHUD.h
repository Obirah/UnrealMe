// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "UnrealMeHUD.generated.h"

UCLASS()
class AUnrealMeHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUnrealMeHUD(const FObjectInitializer& PCIP);
	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};