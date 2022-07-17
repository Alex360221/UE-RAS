// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttachTwoRopes.generated.h"

UCLASS()
class RAS_TRAINER_API AAttachTwoRopes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttachTwoRopes();

	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool attachRope;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* RopeA;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* RopeB;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool detachRopeAtPoint;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* DetachPoint;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool useRotation;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool disablePhysicsOnB;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
