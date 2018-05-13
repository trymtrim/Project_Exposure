//Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableUnit.h"
#include "EngineGlobals.h"
#include "Engine.h"
#include "Engine/World.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
APlaceableUnit::APlaceableUnit ()
{
 	//Set this actor to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void APlaceableUnit::BeginPlay ()
{
	Super::BeginPlay ();

	_playerController = GetWorld ()->GetFirstPlayerController ();
}

//Called every frame
void APlaceableUnit::Tick (float deltaTime)
{
	Super::Tick (deltaTime);

	if (!_isPlaced)
		FollowMousePosition ();
}

void APlaceableUnit::FollowMousePosition ()
{
	//Trace to see what is under the mouse cursor
	FHitResult hit;
	
	_playerController->GetHitResultUnderCursor (ECC_Visibility, true, hit);

	if (hit.bBlockingHit)
	{
		if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Placeable"))
		{
			FVector position = FVector (hit.Location.X, hit.Location.Y, hit.Location.Z + 50.0f);
			SetActorLocation (position);

			//print (hit.GetActor ()->GetName ());
		}
	}
}

void APlaceableUnit::PlaceUnit ()
{
	_isPlaced = true;
}
