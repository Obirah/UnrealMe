// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "UnrealMeKinectV2Connector.h"
#include "UnrealMeTrackingSystemHelper.h"
#include "UnrealMeVRPNConnector.h"
#include "UnrealMeAnimInstance.generated.h"

/**
 * Approach for future improvement, create such enums to replace joint ids.
 */
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
 * Base anim instance class for UnrealMe, provides the basic logic to access tracking data
 * for animations - extendable via Blueprint.
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

	/** Flag indicating whether VRPN is used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	bool iVrpn;

	/** Flag indicating whether pure rotations (instead of rotations 
	calculated from relative locations) should be used */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	bool SkelControl_UsePureRotations;

	/* Spine/neck rotations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_SpineBasePureRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_SpineMidPureRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_NeckPureRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FVector SkelControl_SpineBasePos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FVector SkelControl_SpineMidPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FVector SkelControl_NeckPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_TorsoRotationByShoulders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_TorsoRotationByHips;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_SpineBaseRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_SpineMidRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_NeckRot;

	/* Left arm rotations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftUpperarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftLowerarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftHandRot;

	/* Left thumb and finger rotations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftThumbRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftHandTipRot;

	/* Right arm rotations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightUpperarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightLowerarmRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightHandRot;

	/* Right thumb and finger rotations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightThumbRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightHandTipRot;

	/* Left leg rotations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftThighRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftCalfRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_LeftFootRot;

	/* Right leg rotations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightThighRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightCalfRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UnrealMeAnimation)
	FRotator SkelControl_RightFootRot;

	/** Update the rotation variables with the pure rotations of the joints stemming from a quaternion rotation */
	virtual void updateRotations();
	/** Update the rotation variables with rotations calculated from relative joint positions */
	virtual void updateRotationsByPositions();

	virtual void updateRotationsVRPN();
	/** Update the rotation variables with rotations calculated from relative joint positions */
	virtual void updateRotationsByPositionsVRPN();

	/** Continuation of the enum at the top of this file - example how enums could be used to access joints. */
	UFUNCTION(BlueprintCallable, Category = UnrealMeAnimation)
	static FRotator getJointRotation(EJointRotationType aJoint);

	/** Override the AnimInstance's function that is similar to the PostInitializeComponents event. */
	virtual void NativeInitializeAnimation() override;

	/** Override the AnimInstance's function that is similar to the tick event. */
	virtual void NativeUpdateAnimation(float aDeltaTime) override;
};
