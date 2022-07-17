// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapController.generated.h"

UCLASS()
class RAS_TRAINER_API AMapController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapController();

	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* RopeAttachment;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* WinchController1;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* WinchController2;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* LocationCheckObject;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AttachRopeToWinch();
	void TurnOnWinch();
	void TurnOffWinch();
	void CheckRopePosition();

	bool bCanAttach;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float AttachDistance;
	bool bSwitchWinch;

};
