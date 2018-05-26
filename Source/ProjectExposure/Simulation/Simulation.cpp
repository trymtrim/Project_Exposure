//Fill out your copyright notice in the Description page of Project Settings.

#include "Simulation.h"

//Sets default values
ASimulation::ASimulation ()
{
 	//Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void ASimulation::BeginPlay ()
{
	Super::BeginPlay ();

	InitializeResources ();
}

//Called every frame
void ASimulation::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	UpdateResources (DeltaTime);
}

void ASimulation::OnPlaceUnit (int index)
{
	switch (index)
	{
	case 1: //Nuclear reactor
		AddResources (2, 1);
		break;
	case 2: //Windmill
		AddResources (1, 0);
		break;
	case 3: //Oil rig
		AddResources (3, 3);
		break;
	}
}

void ASimulation::OnNewTurn (int currentTurn)
{

}

void ASimulation::InitializeResources ()
{
	//Maximum resources at start
	maxEnergy = 6;
	maxPollution = 6;

	//Starting resources
	currentEnergy = 0;
	currentPollution = 1;
}

void ASimulation::AddResources (int energyValue, int pollutionValue)
{
	_targetEnergy = currentEnergy + energyValue;
	_targetPollution = currentPollution + pollutionValue;

	_lerping = true;
}

void ASimulation::UpdateResources (float deltaTime)
{
	if (_lerping)
	{
		if (currentEnergy != _targetEnergy || currentPollution != _targetPollution)
		{
			float speed = 1.0f;
			currentEnergy = FMath::FInterpConstantTo (currentEnergy, _targetEnergy, deltaTime, speed);
			currentPollution = FMath::FInterpConstantTo (currentPollution, _targetPollution, deltaTime, speed);
		}
		else
			_lerping = false;
	}
}
