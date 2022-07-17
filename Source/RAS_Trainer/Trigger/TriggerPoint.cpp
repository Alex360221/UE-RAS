// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerPoint.h"
#include "DrawDebugHelpers.h"
#include <RAS_Trainer/Player/PlayerCharacter.h>
#include "Kismet/GameplayStatics.h"
#include "RAS_Trainer/Player/CharacterBase.h"

// Sets default values
ATriggerPoint::ATriggerPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &ATriggerPoint::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerPoint::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ATriggerPoint::BeginPlay()
{
	Super::BeginPlay();


	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, 999, 0, 5);
}

// Called every frame
void ATriggerPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerPoint::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    //ACharacter* other = Cast<ACharacter>(OtherActor);
    //if the overlapping actor is the specific actor we identified in the editor
    //APlayerCharacter* other = Cast<APlayerCharacter>(OtherActor);
    
    //if (OtherActor && OtherActor != this && other/* && OtherActor == SpecificActor*/)
    //{
    //    if (GEngine)
    //    {
    //        GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Begin"));
    //        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Overlapping Actor = %s"), *OverlappedActor->GetName()));
    //    }
    //}





    /*if (OtherActor && (OtherActor != this) && (OtherActor->IsA(ACharacterBase::StaticClass()) == true))
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, FString::Printf(TEXT("%s has entered the trigger"), *OtherActor->GetName()));
    }*/

	ACharacterBase* PlayerCharacter = nullptr;

	if (OtherActor->IsA(ACharacterBase::StaticClass()) == true)
	{
		PlayerCharacter = Cast<ACharacterBase>(OtherActor);
	}

	if (PlayerCharacter != nullptr)
	{
		if (OtherActor && (OtherActor != this))
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, FString::Printf(TEXT("%s has entered the trigger"), *OtherActor->GetName()));
            PlayerCharacter->bInTrigger = true;
		}
	}
}

void ATriggerPoint::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    ////ACharacter* overlap = Cast<ACharacter>(OverlappedActor);
    //APlayerCharacter* other = Cast<APlayerCharacter>(OtherActor);

    ////if the overlapping actor is the specific actor we identified in the editor
    //if (OtherActor && OtherActor != this && other/* && OtherActor == SpecificActor*/)
    //{
    //    if (GEngine)
    //    {
    //        GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Ended"));
    //        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("%s has left the Trigger Box"), *OtherActor->GetName()));
    //    }
    //}

    ACharacterBase* PlayerCharacter = nullptr;

    if (OtherActor->IsA(ACharacterBase::StaticClass()) == true)
    {
        PlayerCharacter = Cast<ACharacterBase>(OtherActor);
    }

	//if (OtherActor && (OtherActor != this) && (OtherActor->IsA(ACharacterBase::StaticClass()) == true))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, FString::Printf(TEXT("%s has left the trigger"), *OtherActor->GetName()));
 //       //OtherActor->bInTrigger = true;
	//}

    if (PlayerCharacter != nullptr)
    {
		if (OtherActor && (OtherActor != this))
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, FString::Printf(TEXT("%s has left the trigger"), *OtherActor->GetName()));
            PlayerCharacter->bInTrigger = false;
		}
    }
    
}

