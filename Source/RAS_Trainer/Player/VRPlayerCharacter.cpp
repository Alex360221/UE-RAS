// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MotionControllerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

AVRPlayerCharacter::AVRPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetCapsuleComponent()->InitCapsuleSize(36.f, 90.f);

	// Base Root Setup
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetupAttachment(GetCapsuleComponent());
	SceneRoot->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	// Camera Root Setup
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(SceneRoot);

	// VR Camera Setup
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(CameraRoot);
	CameraComp->bUsePawnControlRotation = false;
	CameraComp->SetRelativeLocation(FVector(0, 0, 0));

	// Left Motion Control Setup
	MC_Left = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_Left"));
	MC_Left->SetupAttachment(SceneRoot);
	MC_Left->bDisplayDeviceModel = false;						// Set to false if want to hide controller view
	MC_Left->MotionSource = FName(TEXT("Left"));

	LeftHeldObjLoc = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHeldObjLoc"));
	LeftHeldObjLoc->SetupAttachment(MC_Left);
	//LeftHeldObjLoc->SetRelativeLocation(FVector(38, 0, 0));		// Change x value to move closer/further from VR controller position
	LeftHeldObjLoc->SetRelativeLocation(FVector(0, 0, 0));

	// Right Motion Control Setup
	MC_Right = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_Right"));
	MC_Right->SetupAttachment(SceneRoot);
	MC_Right->bDisplayDeviceModel = false;						// Set to false if want to hide controller view
	MC_Right->MotionSource = FName(TEXT("Right"));

	RightHeldObjLoc = CreateDefaultSubobject<USceneComponent>(TEXT("RightHeldObjLoc"));
	RightHeldObjLoc->SetupAttachment(MC_Right);
	//RightHeldObjLoc->SetRelativeLocation(FVector(38, 0, 0));		// Change x value to move closer/further from VR controller position
	RightHeldObjLoc->SetRelativeLocation(FVector(0, 0, 0));

	// Variable Definitions
	GetCharacterMovement()->MaxWalkSpeed = 240.f;
	LineTraceDistance = 100.f;
	bIsLeftGrabbing = false;
	bIsRightGrabbing = false;
	bIsLeftHoldingObj = false;
	bIsRightHoldingObj = false;
	bWasLeftSimulatingPhysics = false;
	bWasRightSimulatingPhysics = false;
}


void AVRPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}


void AVRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AVRPlayerCharacter::LeftPickup()
{
	bIsLeftGrabbing = true;
	
	if (bIsLeftHoldingObj == false)
	{
		FVector Start = MC_Left->GetComponentLocation();
		FVector ForwardVec = MC_Left->GetForwardVector();
		FVector End = ForwardVec * LineTraceDistance;
		End = End + Start;

		FCollisionQueryParams TraceParams;
		//FCollisionObjectQueryParams ObjectParams(ECC_PhysicsBody);
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

		FHitResult Hit = FHitResult(ForceInit);
		//bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectParams, TraceParams);
		bool bIsHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

		if (bIsHit && Hit.GetActor()->ActorHasTag(TAG_PickupTag))
		{
			LeftHitComp = Hit.GetComponent();

			if (LeftHitComp->IsSimulatingPhysics() == true)
			{
				LeftHitComp->SetSimulatePhysics(false);
				SetPhysicsOnActorNetwork(false, LeftHitComp);
				bWasLeftSimulatingPhysics = true;
			}
			LeftPickedUpActor = Hit.GetActor();
			LeftPickedUpActor->AttachToComponent(LeftHeldObjLoc, AttachRules);
			LeftPickedUpActor->SetActorEnableCollision(false);
			SetCollisionsOnActorNetwork(false, LeftPickedUpActor);
			bIsLeftHoldingObj = true;
			bSetActorPos = true;
			ActorToBeSet = LeftPickedUpActor;
		}
	}
}


void AVRPlayerCharacter::RightPickup()
{
	bIsRightGrabbing = true;
	
	if (bIsRightHoldingObj == false)
	{
		FVector Start = MC_Right->GetComponentLocation();
		FVector ForwardVec = MC_Right->GetForwardVector();
		FVector End = ForwardVec * LineTraceDistance;
		End = End + Start;

		FCollisionQueryParams TraceParams;
		//FCollisionObjectQueryParams ObjectParams(ECC_PhysicsBody);
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

		FHitResult Hit = FHitResult(ForceInit);
		//bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectParams, TraceParams);
		bool bIsHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

		if (bIsHit && Hit.GetActor()->ActorHasTag(TAG_PickupTag))
		{
			RightHitComp = Hit.GetComponent();

			if (RightHitComp->IsSimulatingPhysics() == true)
			{
				RightHitComp->SetSimulatePhysics(false);
				SetPhysicsOnActorNetwork(false, RightHitComp);
				bWasRightSimulatingPhysics = true;
			}
			RightPickedUpActor = Hit.GetActor();
			RightPickedUpActor->AttachToComponent(RightHeldObjLoc, AttachRules);
			RightPickedUpActor->SetActorEnableCollision(false);
			SetCollisionsOnActorNetwork(false, RightPickedUpActor);
			bIsRightHoldingObj = true;
			bSetActorPos = true;
			ActorToBeSet = RightPickedUpActor;
		}
	}
}


void AVRPlayerCharacter::LeftDrop()
{
	bIsLeftGrabbing = false;
	
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);

	if (IsValid(LeftPickedUpActor))
	{
		LeftPickedUpActor->DetachFromActor(DetachRules);
		bSetActorPos = false;
		ActorToBeSet = nullptr;

		if (IsValid(LeftHitComp))
		{
			if (bWasLeftSimulatingPhysics == true)
			{
				LeftHitComp->SetSimulatePhysics(true);
				SetPhysicsOnActorNetwork(true, LeftHitComp);
				bWasLeftSimulatingPhysics = false;
			}
			LeftPickedUpActor->SetActorEnableCollision(true);
			SetCollisionsOnActorNetwork(true, LeftPickedUpActor);
			bIsLeftHoldingObj = false;
			LeftHitComp = nullptr;
			LeftPickedUpActor = nullptr;
		}
	}
}


void AVRPlayerCharacter::RightDrop()
{
	bIsRightGrabbing = false;
	
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);

	if (IsValid(RightPickedUpActor))
	{
		RightPickedUpActor->DetachFromActor(DetachRules);
		bSetActorPos = false;
		ActorToBeSet = nullptr;

		if (IsValid(RightHitComp))
		{
			if (bWasRightSimulatingPhysics == true)
			{
				RightHitComp->SetSimulatePhysics(true);
				SetPhysicsOnActorNetwork(true, RightHitComp);
				bWasRightSimulatingPhysics = false;
			}
			RightPickedUpActor->SetActorEnableCollision(true);
			SetCollisionsOnActorNetwork(true, RightPickedUpActor);
			bIsRightHoldingObj = false;
			RightHitComp = nullptr;
			RightPickedUpActor = nullptr;
		}
	}
}


void AVRPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &AVRPlayerCharacter::LeftPickup);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &AVRPlayerCharacter::LeftDrop);

	PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &AVRPlayerCharacter::RightPickup);
	PlayerInputComponent->BindAction("RightGrip", IE_Released, this, &AVRPlayerCharacter::RightDrop);
}

