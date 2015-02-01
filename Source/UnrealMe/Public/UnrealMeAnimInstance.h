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

public:
	UUnrealMeAnimInstance(const FObjectInitializer& PCIP);
	APawn* iOwningPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_SpineBasePureRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_SpineMidPureRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_NeckPureRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FVector SkelControl_SpineBasePos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FVector SkelControl_SpineMidPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FVector SkelControl_NeckPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_TorsoRotationByShoulders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_TorsoRotationByHips;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_SpineBaseRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_SpineMidRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_NeckRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftUpperarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftLowerarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftHandRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightUpperarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightLowerarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightHandRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftThighRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftCalfRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftFootRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightThighRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightCalfRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightFootRot;

	/* Override the AnimInstance's function that is similar to the PostInitializeComponents event. */
	virtual void NativeInitializeAnimation() override;

	/* Override the AnimInstance's function that is similar to the tick event. */
	virtual void NativeUpdateAnimation(float aDeltaTime) override;
};
