// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "VRPlayerCharacter.generated.h"

class USceneComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UMotionControllerComponent;
class UPrimitiveComponent;
class AActor;

/**
 * 
 */
UCLASS()
class RAS_TRAINER_API AVRPlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AVRPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
		USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		USceneComponent* CameraRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionControllers")
		UMotionControllerComponent* MC_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionControllers")
		UMotionControllerComponent* MC_Right;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionControllers")
		USceneComponent* LeftHeldObjLoc;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionControllers")
		USceneComponent* RightHeldObjLoc;

protected:
	virtual void BeginPlay() override;

	void LeftPickup();
	void RightPickup();
	void LeftDrop();
	void RightDrop();

	UPROPERTY(EditAnywhere, Category = "VR")
		float LineTraceDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VR")
		bool bIsLeftGrabbing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VR")
		bool bIsRightGrabbing;

	bool bIsLeftHoldingObj;
	bool bWasLeftSimulatingPhysics;
	bool bIsRightHoldingObj;
	bool bWasRightSimulatingPhysics;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		AActor* LeftPickedUpActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		AActor* RightPickedUpActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		UPrimitiveComponent* LeftHitComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		UPrimitiveComponent* RightHitComp;

private:

};
