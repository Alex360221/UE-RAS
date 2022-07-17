// Fill out your copyright notice in the Description page of Project Settings.


#include "StandardPlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AStandardPlayerCharacter::AStandardPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetCapsuleComponent()->InitCapsuleSize(36.f, 90.f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetRelativeLocation(FVector(0, 0, 50.f));

	HeldObjLoc = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjLoc"));
	HeldObjLoc->SetupAttachment(CameraComp);
	HeldObjLoc->SetRelativeLocation(FVector(100.f, 0, -30.f));

	// Variable Definitions
	LineTraceDistance = 200.f;
	bIsHoldingObj = false;
	bWasSimulatingPhysics = false;
}


void AStandardPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}


void AStandardPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AStandardPlayerCharacter::PickupObject()
{
	if (bIsHoldingObj == false)
	{
		FVector Start = CameraComp->GetComponentLocation();
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FRotator Rot = PC->GetControlRotation();
		FVector End = Rot.Vector() * LineTraceDistance;
		End = Start + End;

		FCollisionQueryParams TraceParams;
		//FCollisionObjectQueryParams ObjectParams(ECC_PhysicsBody);
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		

		FHitResult Hit = FHitResult(ForceInit);
		//bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectParams, TraceParams);
		bool bIsHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

		if (bIsHit && Hit.GetActor()->ActorHasTag(TAG_PickupTag))
		{
			HitComp = Hit.GetComponent();
			
			if (HitComp->IsSimulatingPhysics() == true)
			{
				HitComp->SetSimulatePhysics(false);
				SetPhysicsOnActorNetwork(false, HitComp);
				bWasSimulatingPhysics = true;
			}
			
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, FString::Printf(TEXT("Hit Componenent: %s"), *HitComp->GetName()));
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("HELLO!?")));
			PickedUpActor = Hit.GetActor();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Picked up actor = %s"), *PickedUpActor->GetName()));
			PickedUpActor->AttachToComponent(HeldObjLoc, AttachRules);
			PickedUpActor->SetActorEnableCollision(false);
			SetCollisionsOnActorNetwork(false, PickedUpActor);
			bIsHoldingObj = true;
			bSetActorPos = true;
			ActorToBeSet = PickedUpActor;
		}
	}
}


void AStandardPlayerCharacter::DropObject()
{
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);

	if (IsValid(PickedUpActor))
	{
		PickedUpActor->DetachFromActor(DetachRules);
		bSetActorPos = false;
		ActorToBeSet = nullptr;

		if (IsValid(HitComp))
		{
			if (bWasSimulatingPhysics == true)
			{
				HitComp->SetSimulatePhysics(true);
				SetPhysicsOnActorNetwork(true, HitComp);
				bWasSimulatingPhysics = false;
			}
			PickedUpActor->SetActorEnableCollision(true);
			SetCollisionsOnActorNetwork(true, PickedUpActor);
			bIsHoldingObj = false;
			HitComp = nullptr;
			PickedUpActor = nullptr;
		}
	}
}


void AStandardPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &AStandardPlayerCharacter::PickupObject);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &AStandardPlayerCharacter::DropObject);
}
