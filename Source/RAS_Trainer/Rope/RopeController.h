// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RopeController.generated.h"

UCLASS()
class RAS_TRAINER_API ARopeController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARopeController();
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool ropeControlled;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool tightenRope;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool controlEndOnly;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool controlEndOffAtTarget;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool moveDown;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float tightDistance;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* RopeStart;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* RopeEnd;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		FVector MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float movementSpeed_Float;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* DownwardsTarget;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float distanceFromDownwardsTarget;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		FVector downwardsSpeed;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* endOffTarget;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float distanceFromEndTarget;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* RopePositionControllerRef;
	UPROPERTY(EditAnywhere, Category = "Debugging Stuf")
		int targetIndex;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	bool isRopeTight;
	bool shouldMoveDown;
	FVector endPos;
	bool disabledPhysicsOnEnd;
	bool setTargetIndex;
	class ARopePositionController* ropePositionControllerRef;
	
	void MoveRopeEnds(AActor* start);	//this will move just the start actor
	void MoveRopeEnds(AActor* start, AActor* end);	//this will move just the start actor
	void MoveOnlyRopeEnd(AActor* end);	//this will move just the end actor
	bool CheckDistanceToTarget(AActor* current, AActor* target, float distance);	//this will check if the current is near the closer enough to the target

};
