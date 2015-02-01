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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
    //FVector SkelControl_LeftHandPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FVector SkelControl_SpineBasePos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FVector SkelControl_SpineMidPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FVector SkelControl_NeckPos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_SpineBaseRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_SpineMidRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_NeckRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftElbowRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftWristRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightElbowRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightWristRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftKneeRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftAnkleRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightKneeRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightAnkleRot;

	UFUNCTION(BlueprintCallable, Category = "UnrealMeAnimation")
	//void setLeftHandPos(FVector aPosition);

	/* Override the AnimInstance's function that is similar to the PostInitializeComponents event. */
	virtual void NativeInitializeAnimation() override;

	/* Override the AnimInstance's function that is similar to the tick event. */
	virtual void NativeUpdateAnimation(float aDeltaTime) override;
};
