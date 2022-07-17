// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SpectatorCharacter.generated.h"

class UCameraComponent;

UCLASS()
class RAS_TRAINER_API ASpectatorCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	ASpectatorCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* CameraComp;

protected:
	virtual void BeginPlay() override;

	void MoveUp(float value);
	void ToggleFly();
	void DecreaseFlySpeed();
	void IncreaseFlySpeed();

	bool bIsFlying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float MinFlightSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float MaxFlightSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float SpeedIncrements;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UPROPERTY(VisibleAnywhere, Category = "Ship")
		//TArray<AActor*> Ships;
};
