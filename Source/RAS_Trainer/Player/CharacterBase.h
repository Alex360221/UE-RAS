// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CharacterBase.generated.h"

class AActor;
class UPrimitiveComponent;

UCLASS()
class RAS_TRAINER_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float value);
	void LookUpRate(float value);

	void SwitchCharacter();
	void TriggerTest();
	void AttachToWinch();
	void StartWinch();
	void StopWinch();

	bool bSetActorPos;
	AActor* ActorToBeSet;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Character Switch");
		ACharacterBase* VRCharacter;
	UPROPERTY(EditAnywhere, Category = "Character Switch");
		ACharacterBase* StandardCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		AActor* MapController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
		AActor* RespawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
		float ZRespawnLevel;

	bool bInTrigger;

	UPROPERTY(EditAnywhere, Category = "Tags")
		FName TAG_PickupTag;
	UPROPERTY(EditAnywhere, Category = "Tags")
		FName TAG_ShipTag;

	UFUNCTION(BlueprintNativeEvent, Category = "")
		void MoveActor(FVector NewLocation, FRotator NewRotation, AActor* Actor);
		virtual void MoveActor_Implementation(FVector NewLocation, FRotator NewRotation, AActor* Actor);

	UFUNCTION(Server, Reliable, WithValidation)
		void MoveActorNetwork(FVector NewLocation, FRotator NewRotation, AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, Category = "")
		void SetPhysicsOnActor(bool bSetPhysics, UPrimitiveComponent* Actor);
		virtual void SetPhysicsOnActor_Implementation(bool bSetPhysics, UPrimitiveComponent* Actor);

	UFUNCTION(Server, Reliable, WithValidation)
		void SetPhysicsOnActorNetwork(bool bSetPhysics, UPrimitiveComponent* Actor);

	UFUNCTION(BlueprintNativeEvent, Category = "")
		void SetCollisionsOnActor(bool bSetCollision, AActor* Actor);
		virtual void SetCollisionsOnActor_Implementation(bool bSetCollision, AActor* Actor);

	UFUNCTION(Server, Reliable, WithValidation)
		void SetCollisionsOnActorNetwork(bool bSetCollision, AActor* Actor);
};
