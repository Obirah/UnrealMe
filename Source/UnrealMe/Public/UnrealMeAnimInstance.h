// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "UnrealMeVRPN.h"
#include "UnrealMeKinectV2Connector.h"
#include "UnrealMeVRPNConnector.h"
#include "UnrealMeAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EJointRotationType
{
	JR_SPINE_BY_S UMETA(DisplayName = "Spine Rotation by Shoulders"),
	JR_SPINE_BY_H UMETA(DisplayName = "Spine Rotation by Hips"),

	JR_UPPERARM_L UMETA(DisplayName = "Left Upperarm"),
	JR_LOWERARM_L UMETA(DisplayName = "Left Lowerarm"),
	JR_HAND_L UMETA(DisplayName = "Left Hand"),

	JR_UPPERARM_R UMETA(DisplayName = "Right Upperarm"),
	JR_LOWERARM_R UMETA(DisplayName = "Right Lowerarm"),
	JR_HAND_R UMETA(DisplayName = "Right Hand"),

	JR_THIGH_L UMETA(DisplayName = "Left Thigh"),
	JR_CALF_L UMETA(DisplayName = "Left Calf"),
	JR_FOOT_L UMETA(DisplayName = "Left Foot"),

	JR_THIGH_R UMETA(DisplayName = "Right Thigh"),
	JR_CALF_R UMETA(DisplayName = "Right Calf"),
	JR_FOOT_R UMETA(DisplayName = "Right Foot") 
};

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UNREALME_API UUnrealMeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UUnrealMeVRPNConnector* iVrpnConnector;
public:
	UUnrealMeAnimInstance(const FObjectInitializer& PCIP);
	APawn* iOwningPawn;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	bool SkelControl_UsePureRotations;

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
	FRotator SkelControl_LeftThumbRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_LeftHandTipRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightUpperarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightLowerarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightHandRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightThumbRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KinectV2Animation)
	FRotator SkelControl_RightHandTipRot;

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

	/* Update the rotation variables with the pure rotations of the joints stemming from a quaternion rotation */
	virtual void updateRotations();
	/* Update the rotation variables with rotations calculated from relative joint positions */
	virtual void updateRotationsByPositions();

	virtual void updateRotationsVRPN();
	/* Update the rotation variables with rotations calculated from relative joint positions */
	virtual void updateRotationsByPositionsVRPN();
	/* JUST A TEST: Update with calculated rotations and smoothing */
	virtual void updateRotationsByPositionsWithSmoothing();

	UFUNCTION(BlueprintCallable, Category = KinectV2Animation)
	static FRotator getJointRotation(EJointRotationType aJoint);

	/* Override the AnimInstance's function that is similar to the PostInitializeComponents event. */
	virtual void NativeInitializeAnimation() override;

	/* Override the AnimInstance's function that is similar to the tick event. */
	virtual void NativeUpdateAnimation(float aDeltaTime) override;
};
