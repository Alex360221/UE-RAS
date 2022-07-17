// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

ASpectatorCharacter::ASpectatorCharacter()
{
	//GetCapsuleComponent()->InitCapsuleSize(36.f, 90.f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetRelativeLocation(FVector(0, 0, 50.f));
	
	GetCharacterMovement()->BrakingDecelerationFlying = 1200.f;
	bIsFlying = false;

	GetCharacterMovement()->MaxFlySpeed = 1000.f;
	MinFlightSpeed = 200.f;
	MaxFlightSpeed = 2000.f;
	SpeedIncrements = 200.f;
}


void ASpectatorCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
}


void ASpectatorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float FlightSpeed = GetCharacterMovement()->MaxFlySpeed;
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("MaxFlySpeed = %f"), FlightSpeed));
	//Ships = UGameplayStatics::GetAllActorsWithTag(TAG_ShipTag);
	
}


void ASpectatorCharacter::MoveUp(float value)
{
	if ((Controller) && (value != 0.0f))
	{
		AddMovementInput(GetActorUpVector(), value);
	}
}


void ASpectatorCharacter::ToggleFly()
{
	if (GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Walking;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Changing to walking mode"));
	}
	else if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Changing to flying mode"));
	}
}


void ASpectatorCharacter::DecreaseFlySpeed()
{
	if (GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCharacterMovement()->MaxFlySpeed -= SpeedIncrements;
		GetCharacterMovement()->MaxFlySpeed = FMath::Clamp(GetCharacterMovement()->MaxFlySpeed, MinFlightSpeed, MaxFlightSpeed);
	}
}


void ASpectatorCharacter::IncreaseFlySpeed()
{
	if (GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCharacterMovement()->MaxFlySpeed += SpeedIncrements;
		GetCharacterMovement()->MaxFlySpeed = FMath::Clamp(GetCharacterMovement()->MaxFlySpeed, MinFlightSpeed, MaxFlightSpeed);
	}
}


void ASpectatorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUp", this, &ASpectatorCharacter::MoveUp);

	PlayerInputComponent->BindAction("FKey", IE_Pressed, this, &ASpectatorCharacter::ToggleFly);
	PlayerInputComponent->BindAction("Minus", IE_Pressed, this, &ASpectatorCharacter::DecreaseFlySpeed);
	PlayerInputComponent->BindAction("Plus", IE_Pressed, this, &ASpectatorCharacter::IncreaseFlySpeed);
}
