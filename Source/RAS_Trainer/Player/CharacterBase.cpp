// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "RAS_Trainer/MapController.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "RAS_Trainer/CustomGameInstance.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(36.f, 90.f);

	bInTrigger = false;

	TAG_PickupTag = FName("Pickup");
	TAG_ShipTag = FName("Ship");
	ZRespawnLevel = -100.f;
}


// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSetActorPos && ActorToBeSet)
	{
		MoveActorNetwork(ActorToBeSet->GetActorLocation(), ActorToBeSet->GetActorRotation(), ActorToBeSet);
	}

	if (GetCapsuleComponent()->GetComponentLocation().Z < ZRespawnLevel)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Below respawn level"));
		UGameInstance* Instance = GetGameInstance();
		UCustomGameInstance* GInstance = Cast<UCustomGameInstance>(Instance);
		RespawnPoint = GInstance->RespawnPoint;

		if (RespawnPoint)
		{
			GetCapsuleComponent()->SetWorldLocation(RespawnPoint->GetActorLocation());
		}
	}
}


void ACharacterBase::MoveForward(float value)
{
	if ((Controller) && (value != 0.0f))
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), value);
	}
}


void ACharacterBase::MoveRight(float value)
{
	if ((Controller) && (value != 0.0f))
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}


void ACharacterBase::TurnAtRate(float value)
{
	ACharacterBase::AddControllerYawInput(value);
}


void ACharacterBase::LookUpRate(float value)
{
	ACharacterBase::AddControllerPitchInput(value);
}


void ACharacterBase::SwitchCharacter()
{
	/*APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->UnPossess();
	AActor* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector PlayerLoc = PlayerCharacter->GetActorLocation();
	PlayerCharacter->Destroy();
	AStandardPlayerCharacter* NewCharacter = CreateDefaultSubobject<AStandardPlayerCharacter>();
	GetWorld()->SpawnActor(NewCharacter, PlayerLoc);*/
}


void ACharacterBase::TriggerTest()
{
	if (bInTrigger == true)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("In trigger and pressed key")));
	}
	else 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Not in trigger")));
	}
	
}


void ACharacterBase::AttachToWinch()
{
	AMapController* MControl = Cast<AMapController>(MapController);

	if (MControl)
	{
		MControl->AttachRopeToWinch();
	}
}


void ACharacterBase::StartWinch()
{
	AMapController* MControl = Cast<AMapController>(MapController);

	if (MControl)
	{
		MControl->TurnOnWinch();
	}
}


void ACharacterBase::StopWinch()
{
	AMapController* MControl = Cast<AMapController>(MapController);

	if (MControl)
	{
		MControl->TurnOffWinch();
	}
}


// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Player Movement
	PlayerInputComponent->BindAxis("MoveX", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveY", this, &ACharacterBase::MoveRight);

	// Player Look Movement
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterBase::LookUpRate);

	PlayerInputComponent->BindAction("B Key", IE_Pressed, this, &ACharacterBase::AttachToWinch);
	PlayerInputComponent->BindAction("N Key", IE_Pressed, this, &ACharacterBase::StartWinch);
	PlayerInputComponent->BindAction("M Key", IE_Pressed, this, &ACharacterBase::StopWinch);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);//uses the inbuilt jump function
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAction("FKey", IE_Pressed, this, &APlayerCharacter::FKey);

	//PlayerInputComponent->BindAction("SwitchCharacter", IE_Pressed, this, &ACharacterBase::SwitchCharacter);

	//PlayerInputComponent->BindAction("TriggerTest", IE_Pressed, this, &ACharacterBase::TriggerTest);
}


void ACharacterBase::MoveActor_Implementation(FVector NewLocation, FRotator NewRotation, AActor* Actor)
{
	if (Actor)
	{
		Actor->SetActorLocationAndRotation(NewLocation, NewRotation);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Moved actor"));
	}
}


void ACharacterBase::MoveActorNetwork_Implementation(FVector NewLocation, FRotator NewRotation, AActor* Actor)
{
	MoveActor(NewLocation, NewRotation, Actor);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, TEXT("Network Moved actor"));
}


bool ACharacterBase::MoveActorNetwork_Validate(FVector NewLocation, FRotator NewRotation, AActor* Actor)
{
	return true;
}


void ACharacterBase::SetPhysicsOnActor_Implementation(bool bSetPhysics, UPrimitiveComponent* Actor)
{
	if (Actor)
	{
		Actor->SetSimulatePhysics(bSetPhysics);
	}
}


void ACharacterBase::SetPhysicsOnActorNetwork_Implementation(bool bSetPhysics, UPrimitiveComponent* Actor)
{
	SetPhysicsOnActor(bSetPhysics, Actor);
}


bool ACharacterBase::SetPhysicsOnActorNetwork_Validate(bool bSetPhysics, UPrimitiveComponent* Actor)
{
	return true;
}


void ACharacterBase::SetCollisionsOnActor_Implementation(bool bSetCollision, AActor* Actor)
{
	if (Actor)
	{
		Actor->SetActorEnableCollision(bSetCollision);
	}
}


void ACharacterBase::SetCollisionsOnActorNetwork_Implementation(bool bSetCollision, AActor* Actor)
{
	SetCollisionsOnActor(bSetCollision, Actor);
}


bool ACharacterBase::SetCollisionsOnActorNetwork_Validate(bool bSetCollision, AActor* Actor)
{
	return true;
}

