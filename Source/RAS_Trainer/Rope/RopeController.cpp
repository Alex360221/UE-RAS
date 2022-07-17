// Fill out your copyright notice in the Description page of Project Settings.


#include "RopeController.h"
#include "RopePositionController.h"

// Sets default values
ARopeController::ARopeController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//public varaibles
	ropeControlled = false;
	tightenRope = false;
	moveDown = false;
	controlEndOnly = false;
	controlEndOffAtTarget = false;
	tightDistance = 6000.0f;
	MovementSpeed = FVector(0.1, 0, 0);
	downwardsSpeed = FVector(0, 0, 0.1f);
	distanceFromDownwardsTarget = 100.0f;
	distanceFromEndTarget = 50.0f;

	//private varaibels
	isRopeTight = false;
	shouldMoveDown = false;
	disabledPhysicsOnEnd = false;
	setTargetIndex = true;

	endPos = FVector(0, 0, 0);

}

// Called when the game starts or when spawned
void ARopeController::BeginPlay()
{
	Super::BeginPlay();
	if (RopeEnd) { endPos = RopeEnd->GetActorLocation(); }
	ropePositionControllerRef = Cast<ARopePositionController>(RopePositionControllerRef);
}

// Called every frame
void ARopeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ropeControlled)
	{
		//controll the start and end points of the rope
		if (RopeStart && RopeEnd)
		{
			if (moveDown && !DownwardsTarget) { return; }
			//if tighten rope == true and rope is not yet tight
			if (tightenRope && !isRopeTight)
			{
				//GLog->Log("Rope is tightering");
				MoveRopeEnds(RopeStart);
				float dis = FVector::Dist(RopeStart->GetActorLocation(), RopeEnd->GetActorLocation());
				if (dis >= tightDistance)
				{
					isRopeTight = true;
				}
			}
			else if(tightenRope)
			{
				//GLog->Log("Rope is tight, ove both peices");
				MoveRopeEnds(RopeStart, RopeEnd);
			}	
			else
			{
				//GLog->Log("Moving rope");
				MoveRopeEnds(RopeStart, RopeEnd);
			}
		}
	}
	if (controlEndOnly)
	{
		//check distance to target
		if (controlEndOffAtTarget && CheckDistanceToTarget(RopeEnd, endOffTarget,distanceFromEndTarget) && !disabledPhysicsOnEnd)
		{
			UActorComponent* comp = RopeEnd->GetComponentByClass(UStaticMeshComponent::StaticClass());
			UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
			if (sm)
			{
				//GLog->Log("Enable physics!");
				disabledPhysicsOnEnd = true;
				sm->SetSimulatePhysics(true);
			}
		}
		else if(!disabledPhysicsOnEnd)
		{
			MoveOnlyRopeEnd(RopeEnd);
		}		
	}

}

void ARopeController::MoveRopeEnds(AActor* start)
{
	if (!start) { return; }
	if (moveDown)
	{
		if (!DownwardsTarget) { return; }
		if (!shouldMoveDown)
		{//work out if its closer enough so that it should move down			
			float dis = FVector::Dist(start->GetActorLocation(), DownwardsTarget->GetActorLocation());
			if (dis <= distanceFromDownwardsTarget)
			{
				//GLog->Log("Within distance move down");
				shouldMoveDown = true;
				start->SetActorLocation(start->GetActorLocation() + downwardsSpeed);
				return;
			}
		}
		else
		{
			//GLog->Log("Within distance move down one111");
			start->SetActorLocation(start->GetActorLocation() + downwardsSpeed);
			return;
		}

	}
	start->SetActorLocation(start->GetActorLocation() + MovementSpeed);
}

void ARopeController::MoveRopeEnds(AActor* start, AActor* end)
{
	if (!start) { return; }
	//check the end of the rope hasn't passed the downwards target pos
	float dis = FVector::Dist(end->GetActorLocation(), DownwardsTarget->GetActorLocation());
	if (dis <= distanceFromDownwardsTarget)
	{
		ropeControlled = false;
		return;
	}
	if (moveDown)
	{
		if (!DownwardsTarget) { return; }
		if (!shouldMoveDown)
		{//work out if its closer enough so that it should move down			
			dis = FVector::Dist(start->GetActorLocation(), DownwardsTarget->GetActorLocation());
			if (dis <= distanceFromDownwardsTarget)
			{
				shouldMoveDown = true;
				//GLog->Log("Within distance move down");
				start->SetActorLocation(start->GetActorLocation() + downwardsSpeed);
				end->SetActorLocation(end->GetActorLocation() + MovementSpeed);
				return;
			}
			start->SetActorLocation(start->GetActorLocation() + MovementSpeed);
			end->SetActorLocation(end->GetActorLocation() + MovementSpeed);
		}
		else
		{
			//GLog->Log("Within distance move down");
			//GLog->Log("Within distance move down two22222222222222");
			start->SetActorLocation(start->GetActorLocation() + downwardsSpeed);
			end->SetActorLocation(end->GetActorLocation() + MovementSpeed);
		}
		return;
	}
	start->SetActorLocation(start->GetActorLocation() + MovementSpeed);
	end->SetActorLocation(end->GetActorLocation() + MovementSpeed);
}

void ARopeController::MoveOnlyRopeEnd(AActor* end)
{
	if (end && endOffTarget)
	{
		//if has rope holders then use those as target positions
		if (ropePositionControllerRef)
		{
			//GLog->Log("Lerping stuff!!");
			if (setTargetIndex && ropePositionControllerRef->objects.Num() > 0)
			{ 
				//GLog->Log("Set Index");
				setTargetIndex = false;
				targetIndex = ropePositionControllerRef->objects.Num() - 1;
			}
			//get distance to target
			FVector targetPosition = FVector(0, 0, 0);
			if (targetIndex <= 0) { targetPosition = endOffTarget->GetActorLocation(); }
			else { targetPosition = ropePositionControllerRef->objects[targetIndex]->GetActorLocation(); }

			float dis = FVector::Dist(targetPosition, RopeEnd->GetActorLocation());
			if (dis <= 50)
			{
				targetIndex -= 1;
			}
			//get directional vector to target
			FVector direction = targetPosition - RopeEnd->GetActorLocation();
			direction.Normalize();
			endPos += direction * movementSpeed_Float;
			end->SetActorLocation(endPos);
			return;
		}
		//get directional vector to target
		//FVector direction = endOffTarget->GetActorLocation() - RopeEnd->GetActorLocation();
		//direction.Normalize();
		endPos += MovementSpeed;
		end->SetActorLocation(endPos);
	}
}

bool ARopeController::CheckDistanceToTarget(AActor* current, AActor* target, float distance)
{
	float dis = FVector::Dist(current->GetActorLocation(), target->GetActorLocation());
	if (dis <= distance)
	{
		return true;
	}
	return false;
}

