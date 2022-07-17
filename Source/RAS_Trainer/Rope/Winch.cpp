// Fill out your copyright notice in the Description page of Project Settings.


#include "Winch.h"
//#include "MotionControllerComponent.h"

// Sets default values
AWinch::AWinch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	turnOn = true;
	rotationAxis = FVector(1, 0, 0);
	rotationSpeed = 10.0f;
}

// Called when the game starts or when spawned
void AWinch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ObjectToSpin && turnOn)
	{
		FQuat axis = FQuat(rotationAxis,rotationSpeed);
		FQuat rotation = FQuat(ObjectToSpin->GetActorRotation());
		ObjectToSpin->SetActorRotation(rotation * axis);
	}

}

