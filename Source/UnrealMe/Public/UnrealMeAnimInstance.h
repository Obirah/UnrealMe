// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "UnrealMeVRPN.h"
#include "UnrealMeKinectV2Connector.h"
#include "UnrealMeAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UNREALME_API UUnrealMeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	//UUnrealMeVRPN* iVRPNClient;

public:
	UUnrealMeAnimInstance(const FObjectInitializer& PCIP);
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HandPlacement)
	APawn* iOwningPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HandPlacement)
    FVector SkelControl_LeftHandPos;

	UFUNCTION(BlueprintCallable, Category = "UnrealMeAnimation")
	void setLeftHandPos(FVector aPosition);

	/* Override the AnimInstance's function that is similar to the PostInitializeComponents event. */
	virtual void NativeInitializeAnimation() override;

	/* Override the AnimInstance's function that is similar to the tick event. */
	virtual void NativeUpdateAnimation(float aDeltaTime) override;
};
