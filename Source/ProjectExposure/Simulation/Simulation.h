//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Simulation.generated.h"

UCLASS()
class PROJECTEXPOSURE_API ASimulation : public AActor
{
	GENERATED_BODY ()
	
public:	
	//Sets default values for this actor's properties
	ASimulation ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called from SimulationGameController when a unit is placed
	void OnPlaceUnit (int index);

	void OnNewTurn (int currentTurn);

	//Values for the energy and pollution bars //To make blocks, put a transparent sprite in front of the bars or something
	UPROPERTY (BlueprintReadOnly)
	float maxEnergy;
	UPROPERTY (BlueprintReadOnly)
	float currentEnergy;
	UPROPERTY (BlueprintReadOnly)
	float maxPollution;
	UPROPERTY (BlueprintReadOnly)
	float currentPollution;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void InitializeResources ();
	void AddResources (int energyValue, int pollutionValue);
	void UpdateResources (float deltaTime);

	//For lerping between values
	float _targetEnergy;
	float _targetPollution;
	bool _lerping;
};
