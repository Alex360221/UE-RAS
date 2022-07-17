// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RAS_TRAINER_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Ship")
		FVector ShipDirection;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Ship")
		float ShipSpeed;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Respawn Point")
		AActor* RespawnPoint;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Map Controller")
		AActor* MapController;
};
