// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachTwoRopes.h"

// Sets default values
AAttachTwoRopes::AAttachTwoRopes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	attachRope = false;
	detachRopeAtPoint = false;
	useRotation = false;
}

// Called when the game starts or when spawned
void AAttachTwoRopes::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttachTwoRopes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (detachRopeAtPoint && DetachPoint)
	{
		float dis = FVector::Dist(RopeB->GetActorLocation(), DetachPoint->GetActorLocation());
		if (dis < 50)
		{
			attachRope = false;
			detachRopeAtPoint = false;
		}
		//RopeB->
	}
	if (disablePhysicsOnB)
	{
		//disables physics for this object
		disablePhysicsOnB = false;
		UActorComponent* comp = RopeB->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
		if (sm)
		{
			//GLog->Log("Enable physics!");
			sm->SetSimulatePhysics(false);
		}
	}

	if (attachRope)
	{
		if (RopeA && RopeB)
		{
			FQuat rotation = FQuat();
			if (useRotation) { rotation = FQuat(RopeA->GetActorRotation()); }
			RopeB->SetActorLocation(RopeA->GetActorLocation() + (rotation * FVector(10,0,0)));
		}
	}
}

