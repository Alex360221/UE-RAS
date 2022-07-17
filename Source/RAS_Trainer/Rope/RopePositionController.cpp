// Fill out your copyright notice in the Description page of Project Settings.


#include "RopePositionController.h"
#include "RopeHolder.h"

// Sets default values
ARopePositionController::ARopePositionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARopePositionController::BeginPlay()
{
	Super::BeginPlay();
	SpawnObjectsBetweenShips();
}

// Called every frame
void ARopePositionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateObjectLocations();

}

void ARopePositionController::SpawnObjectsBetweenShips()
{
	if (!edgeOfShipA && !edgeOfShipB) { return; }
	AStart = edgeOfShipA->GetActorLocation();
	BStart = edgeOfShipB->GetActorLocation();

	//GLog->Log("Create laall");
	float dis = FVector::Distance(edgeOfShipA->GetActorLocation(), edgeOfShipB->GetActorLocation());
	int amountOfObjects = dis / objectLength;
	FVector direction = edgeOfShipB->GetActorLocation() - edgeOfShipA->GetActorLocation();
	direction.Normalize();
	for (int i = 0; i <= amountOfObjects;i++)
	{
		FVector spawnPos = edgeOfShipA->GetActorLocation() + ((i * objectLength) * direction);
		objects.Add(GetWorld()->SpawnActor<ARopeHolder>(object, spawnPos, FRotator(0, 0, 0)));
		//GetWorld()->SpawnActor(object,)
		//GLog->Log("Spawning Object");
	}
}

void ARopePositionController::UpdateObjectLocations()
{
	if (!edgeOfShipA && !edgeOfShipB) { return; }
	if (MoveUp)
	{
		FVector ATarget = FVector(AStart.X, AStart.Y, AStart.Z + AddedOntoZ);
		FVector BTarget = FVector(BStart.X, BStart.Y, BStart.Z + AddedOntoZ);
		float disA = FVector::Distance(edgeOfShipA->GetActorLocation(), ATarget);
		if (disA > 5)
		{
			//GLog->Log("Update");
			FVector direction = edgeOfShipA->GetActorLocation() - ATarget;
			direction.Normalize();
			edgeOfShipA->SetActorLocation(edgeOfShipA->GetActorLocation() + (FVector(0,0, AddedOntoZ) * Speed));
			
			direction = edgeOfShipB->GetActorLocation() - BTarget;
			direction.Normalize();
			edgeOfShipB->SetActorLocation(edgeOfShipB->GetActorLocation() + (FVector(0, 0, AddedOntoZ) * Speed));
		}
		else
		{
			MoveUp = false;
		}
	}
	float dis = FVector::Distance(edgeOfShipA->GetActorLocation(), edgeOfShipB->GetActorLocation());
	int amountOfObjects = dis / objectLength;
	FVector direction = edgeOfShipB->GetActorLocation() - edgeOfShipA->GetActorLocation();
	direction.Normalize();
	for (int i = 0; i < objects.Num();i++)
	{
		FVector spawnPos = edgeOfShipA->GetActorLocation() + ((i * objectLength) * direction);
		//objects.Add(GetWorld()->SpawnActor<ARopeHolder>(object, spawnPos, FRotator(0, 0, 0)));
		objects[i]->SetActorLocation(spawnPos);		
	}
}

