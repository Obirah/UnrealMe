// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UnrealPit_UE4.h"
#include "UnrealPit_UE4Character.h"
#include "UnrealPit_UE4Projectile.h"
#include "Animation/AnimInstance.h"


//////////////////////////////////////////////////////////////////////////
// AUnrealPit_UE4Character

AUnrealPit_UE4Character::AUnrealPit_UE4Character(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AActor::SetActorTickEnabled(true);

	tVRPNClient = NewObject<UUnrealPit_UE4VRPN>();
	tVRPNClient->initializeTracker();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealPit_UE4Character::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	InputComponent->BindAction("Fire", IE_Pressed, this, &AUnrealPit_UE4Character::OnFire);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUnrealPit_UE4Character::TouchStarted);

	InputComponent->BindAxis("MoveForward", this, &AUnrealPit_UE4Character::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUnrealPit_UE4Character::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AUnrealPit_UE4Character::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AUnrealPit_UE4Character::LookUpAtRate);
}

void AUnrealPit_UE4Character::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<AUnrealPit_UE4Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

}

void AUnrealPit_UE4Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// only fire for first finger down
	if (FingerIndex == 0)
	{
		OnFire();
	}
}

void AUnrealPit_UE4Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUnrealPit_UE4Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUnrealPit_UE4Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealPit_UE4Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealPit_UE4Character::Tick(float aDeltaSeconds)
{
	FVector* tHeadPos = tVRPNClient->getKinectBonePosition(0);
	FVector* tTorsoPos = tVRPNClient->getKinectBonePosition(2);
	FVector* tLHPos = tVRPNClient->getKinectBonePosition(8);
	FVector* tRHPos = tVRPNClient->getKinectBonePosition(14);

	UE_LOG(UnrealPit, Log, TEXT("Head Position from VRPNClient: %s"), *tHeadPos->ToString());
	UE_LOG(UnrealPit, Log, TEXT("Torso Position from VRPNClient: %s"), *tTorsoPos->ToString());
	UE_LOG(UnrealPit, Log, TEXT("Left Hand Position from VRPNClient: %s"), *tLHPos->ToString());
	UE_LOG(UnrealPit, Log, TEXT("Right Hand Position from VRPNClient: %s"), *tRHPos->ToString());
}

