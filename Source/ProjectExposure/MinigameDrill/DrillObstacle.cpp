//Fill out your copyright notice in the Description page of Project Settings.

#include "DrillObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "MinigameDrillController.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
ADrillObstacle::ADrillObstacle ()
{
 	//Set this actor to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void ADrillObstacle::BeginPlay ()
{
	Super::BeginPlay ();
}

//Called every frame
void ADrillObstacle::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	SetActorLocation (GetActorLocation () + (FVector (0.0f, 0.0f, 400.0f) * DeltaTime));

	FVector position = GetActorLocation ();

	if (position.Z > _collideHeight && !_hasCollided)
	{
		if (_gameController->GetCurrentDrillType () == _type)
			Destroy ();
		else
		{
			_gameController->GetHitByObstacle ();
			_hasCollided = true;
		}
	}

	if (position.Z > -100.0f)
		Destroy ();
}

void ADrillObstacle::Initialize (AActor* drill, AMinigameDrillController* gameController)
{
	_gameController = gameController;

	//Set type based on tag
	TArray <UStaticMeshComponent*> staticComps;
	GetComponents <UStaticMeshComponent> (staticComps);

	FString type = staticComps [0]->ComponentTags [0].ToString ();

	if (type == "1")
		_type = 1;
	else if (type == "2")
		_type = 2;
	else if (type == "3")
		_type = 3;

	//Set collide height
	TArray <UStaticMeshComponent*> drillStaticComps;
	drill->GetComponents <UStaticMeshComponent> (drillStaticComps);

	_collideHeight = drill->GetActorLocation ().Z - (drillStaticComps [0]->CalcBounds (drill->GetTransform ()).BoxExtent.Z) - (staticComps [0]->CalcBounds (GetTransform ()).BoxExtent.Z);
}
