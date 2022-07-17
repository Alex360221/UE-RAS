// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
//#include <Engine/Source/Runtime/HeadMountedDisplay/Public/MotionControllerComponent.h>
#include "PlayerCharacter.generated.h"

class UMotionControllerComponent;
class UHeadMountedDisplayFunctionLibrary;

UCLASS()
class RAS_TRAINER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
		USceneComponent* VRRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		USceneComponent* VRCameraRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* VRCameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* FPCameraComp;
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float value);
	void LookUpRate(float value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	bool FkeyPressed;
	bool doneLineTrace;
	UPrimitiveComponent* hitObject;
	FName boneName;
	UHeadMountedDisplayFunctionLibrary* hmd;
	void FKey();
	void LineTrace();
	void EnableVR();
	void DisableVR();

};
