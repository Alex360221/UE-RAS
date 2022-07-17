// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "StandardPlayerCharacter.generated.h"

class UCameraComponent;
class USceneComponent;
class UPrimitiveComponent;
class AActor;

/**
 * 
 */
UCLASS()
class RAS_TRAINER_API AStandardPlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AStandardPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		USceneComponent* HeldObjLoc;

protected:
	virtual void BeginPlay() override;

	void PickupObject();
	void DropObject();

	UPROPERTY(EditAnywhere, Category = "Line Trace")
	float LineTraceDistance;

	bool bIsHoldingObj;
	bool bWasSimulatingPhysics;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		AActor* PickedUpActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		UPrimitiveComponent* HitComp;
};
