// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "RopeHolder.h"
#include "RopePositionController.generated.h"

UCLASS()
class RAS_TRAINER_API ARopePositionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARopePositionController();

	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* edgeOfShipA;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		AActor* edgeOfShipB;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		TSubclassOf<class ARopeHolder> object;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float objectLength;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		TArray<AActor*> objects;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float AddedOntoZ;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		float Speed;

	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		bool MoveUp;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		FVector AStart;
	UPROPERTY(EditAnywhere, Category = "Rope Settings")
		FVector BStart;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void SpawnObjectsBetweenShips();
	void UpdateObjectLocations();
	

};
