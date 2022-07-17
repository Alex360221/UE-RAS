// Fill out your copyright notice in the Description page of Project Settings.


#include "MapController.h"
#include "Rope/AttachTwoRopes.h"
#include "Rope/Winch.h"

// Sets default values
AMapController::AMapController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanAttach = false;
	bSwitchWinch = false;
	AttachDistance = 50.f;
}

// Called when the game starts or when spawned
void AMapController::BeginPlay()
{
	Super::BeginPlay();
	

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Can attach rope = %s"), bCanAttach ? TEXT("True") : TEXT("False")));
}

// Called every frame
void AMapController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapController::AttachRopeToWinch()
{
	AAttachTwoRopes* atr = Cast<AAttachTwoRopes>(RopeAttachment);
	
	CheckRopePosition();
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Attempting to attach winch"));

	if (atr && bCanAttach)
	{
		atr->attachRope = true;
		bSwitchWinch = true;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Attach winch successful"));
	}
}

void AMapController::TurnOnWinch()
{
	AWinch* winch;
	if (!bSwitchWinch) { winch = Cast<AWinch>(WinchController1); }
	else { winch = Cast<AWinch>(WinchController2); }
		

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("About to start winch"));

	if (winch)
	{
		winch->turnOn = true;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Starting"));
	}
}

void AMapController::TurnOffWinch()
{
	AWinch* winch;
	if (!bSwitchWinch) { winch = Cast<AWinch>(WinchController1); }
	else { winch = Cast<AWinch>(WinchController2); }

	
	if (winch)
	{
		winch->turnOn = false;
	}
}


void AMapController::CheckRopePosition()
{
	AAttachTwoRopes* atr = Cast<AAttachTwoRopes>(RopeAttachment);
	if (atr && LocationCheckObject)
	{
		FVector RopeEndPos = atr->RopeB->GetActorLocation();
		FVector CheckPos = LocationCheckObject->GetActorLocation();
		if (FVector::Distance(RopeEndPos, CheckPos) < AttachDistance)
		{
			bCanAttach = true;
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Within Distance"));
		}
		else
		{
			bCanAttach = false;
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Not within distance"));
		}
		
		/*FVector VectorBetween = RopeEndPos - CheckPos;
		float DistanceBetween = VectorBetween.SizeSquared();*/
	}
}

