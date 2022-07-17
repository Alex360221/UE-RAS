// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PoseableMeshComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(20.f, 40.0f);

	// Base Root Setup
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	VRRoot->SetupAttachment(GetCapsuleComponent());
	VRRoot->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight())); // minus capsule half height

	// Camera Root Setup
	VRCameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraRoot"));
	VRCameraRoot->SetupAttachment(VRRoot);

	// VR Camera Setup
	VRCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCameraComp"));
	VRCameraComp->SetupAttachment(VRCameraRoot);
	//VRCameraComp->bUsePawnControlRotation = true;
	VRCameraComp->bUsePawnControlRotation = false;
	VRCameraComp->SetRelativeLocation(FVector(0, 0, 0));

	// First Person Camera Setup
	FPCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCameraComp"));
	FPCameraComp->SetupAttachment(GetCapsuleComponent());
	FPCameraComp->bUsePawnControlRotation = true;
	FPCameraComp->SetRelativeLocation(FVector(0, 0, 0));
	FPCameraComp->SetActive(false);

	// Left Motion Control Setup
	MC_Left = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_Left"));
	MC_Left->SetupAttachment(VRRoot);
	MC_Left->bDisplayDeviceModel = true;
	MC_Left->MotionSource = FName(TEXT("Left"));
	LeftHeldObjLoc = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHeldObjLoc"));
	LeftHeldObjLoc->SetupAttachment(MC_Left);
	LeftHeldObjLoc->SetRelativeLocation(FVector(38, 0, 0));

	// Right Motion Control Setup
	MC_Right = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_Right"));
	MC_Right->SetupAttachment(VRRoot);
	MC_Right->bDisplayDeviceModel = true;
	MC_Right->MotionSource = FName(TEXT("Right"));
	RightHeldObjLoc = CreateDefaultSubobject<USceneComponent>(TEXT("RightHeldObjLoc"));
	RightHeldObjLoc->SetupAttachment(MC_Right);
	RightHeldObjLoc->SetRelativeLocation(FVector(38, 0, 0));

	// Variable Definitions
	FkeyPressed = false;
	doneLineTrace = false;

	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (FkeyPressed)
	{
		LineTrace();
	}*/

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CameraComp Rotation: %s"), *CameraComp->GetRelativeRotation().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("VRCameraRoot Rotation: %s"), *VRCameraRoot->GetRelativeRotation().ToString()));
	//UE_LOG(LogTemp, Warning, TEXT("CameraComp Rotation: %s"), *VRCameraComp->GetRelativeRotation().ToString());

	if (hmd->IsHeadMountedDisplayEnabled() == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = 240.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Left Motion Controller Pos: %s"), *MC_Left->GetRelativeLocation().ToString()));
}


void APlayerCharacter::MoveForward(float value)
{
	if ((Controller) && (value != 0.0f))
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if ((Controller) && (value != 0.0f))
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void APlayerCharacter::TurnAtRate(float value)
{
	APlayerCharacter::AddControllerYawInput(value);
}

void APlayerCharacter::LookUpRate(float value)
{
	APlayerCharacter::AddControllerPitchInput(value);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);//uses the inbuilt jump function
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("FKey", IE_Pressed, this, &APlayerCharacter::FKey);

	// Player Movement
	InputComponent->BindAxis("MoveX", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveY", this, &APlayerCharacter::MoveRight);

	// Player Look Movement
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUpRate);

	// Enable/Disable HMD
	PlayerInputComponent->BindAction("EnableHMD", IE_Pressed, this, &APlayerCharacter::EnableVR);
	PlayerInputComponent->BindAction("DisableHMD", IE_Pressed, this, &APlayerCharacter::DisableVR);
}

void APlayerCharacter::FKey()
{
	doneLineTrace = false;
	if (FkeyPressed)
	{
		FkeyPressed = false;	
	}
	else
	{
		FkeyPressed = true;
	}
}

void APlayerCharacter::LineTrace()
{
	//GLog->Log("Line Trace add");
	FVector start = VRCameraComp->GetComponentLocation();
	FVector forwardVector = VRCameraComp->GetForwardVector();
	FVector end = (forwardVector * 200) + start;
	FCollisionQueryParams collisionsParms;
	if (!doneLineTrace)
	{
		//DrawDebugLine(GetWorld(), start, end, FColor::Green, true);
		FHitResult outHit;
		bool isHit = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, collisionsParms);

		if (isHit)
		{
			if (outHit.GetActor())
			{
				doneLineTrace = true;
				hitObject = outHit.GetComponent();
				boneName = outHit.BoneName;
				//GLog->Log("Moving Ubject : " + hitObject->GetName());
				//outHit.GetActor()->SetActorLocation(end, false, nullptr, ETeleportType::TeleportPhysics);
			}
		}
	}
	if (hitObject != nullptr)
	{
		//hitObject->SetActorLocation(end, false, nullptr, ETeleportType::TeleportPhysics);
		UPrimitiveComponent* PrimMesh = Cast<UPrimitiveComponent>(hitObject);
		if (PrimMesh != nullptr)
		{
			//GLog->Log("PrimMesh MEHS!!!!");
			UPoseableMeshComponent* poseMesh = Cast<UPoseableMeshComponent>(PrimMesh);
			if (poseMesh)
			{
				//GLog->Log("ayayyaysyayyayaayyayyayyay");
			}
			//skeletalMesh->SetBoneLocationByName(boneName, end, EBoneSpaces::WorldSpace);
		}
		else
		{
			//GLog->Log("cast fail");
			doneLineTrace = false;
		}
	}
	
}

void APlayerCharacter::EnableVR()
{
	if (hmd->IsHeadMountedDisplayEnabled() == false)
	{
		//VRCameraComp->bUsePawnControlRotation = false;
		////VRRoot->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight())); // minus capsule half height
		//VRCameraComp->SetRelativeLocation(FVector(0, 0, 0));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CameraComp Location: %s"), *VRCameraComp->GetRelativeLocation().ToString()));
		//MC_Left->Activate();
		//MC_Right->Activate();





		hmd->EnableHMD(true);

		FPCameraComp->SetActive(false);
		VRCameraComp->SetActive(true);
		
		if (MC_Left != nullptr && MC_Right != nullptr)
		{
			MC_Left->SetVisibility(true, true);
			MC_Right->SetVisibility(true, true);
		}
		
		
		
	}
}

void APlayerCharacter::DisableVR()
{
	if (hmd->IsHeadMountedDisplayEnabled() == true)
	{
		
		////VRRoot->SetRelativeLocation(FVector(0, 0, 0));
		//VRCameraComp->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CameraComp Location: %s"), *VRCameraComp->GetRelativeLocation().ToString()));
		//MC_Left->Deactivate();
		//MC_Right->Deactivate();

		//FRotator rot = VRCameraComp->GetRelativeRotation();
		//VRCameraComp->SetRelativeRotation(FRotator(0, rot.Yaw, 0));
		////CameraComp->SetRelativeRotation(FRotator(0, 0, 0));
		////CameraComp->SetWorldRotation(FRotator(0, 0, 0));
		////CameraComp->SetWorldRotation(FRotator(0, rot.Yaw, 0));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CameraComp Rotation: %s"), *VRCameraComp->GetRelativeRotation().ToString()));
		//
		//VRCameraComp->bUsePawnControlRotation = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("2nd CameraComp Rotation: %s"), *VRCameraComp->GetRelativeRotation().ToString()));
		



		hmd->EnableHMD(false);

		VRCameraComp->SetActive(false);
		FPCameraComp->SetActive(true);
		FPCameraComp->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
		
		if (MC_Left != nullptr && MC_Right != nullptr)
		{
			MC_Left->SetVisibility(false, true);
			MC_Right->SetVisibility(false, true);
		}		
	}
}

