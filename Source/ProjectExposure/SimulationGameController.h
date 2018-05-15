//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlaceableUnit.h"
#include "SimulationGameController.generated.h"

UCLASS()
class PROJECTEXPOSURE_API ASimulationGameController : public APawn
{
	GENERATED_BODY ()

public:
	ASimulationGameController ();

public:	
	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	//For blueprints
	UFUNCTION (BlueprintCallable, Category = "ButtonPress")
	void SpawnUnit (int index);
	//
	UPROPERTY (BlueprintReadWrite)
	bool showUI = true;
	UPROPERTY (BlueprintReadWrite)
	bool showSimulationUI = false;

	UPROPERTY (BlueprintReadWrite)
	FString currentTurnText = "Turn 1";

protected:
	virtual void BeginPlay () override;

private:
	//Places the unit that is currently being controlled
	void PlaceUnit ();

	//Currently controlled unit
	APlaceableUnit* _controlledUnit = nullptr;
	
	//Simulation setup
	int _maxTurns = 9;
	int _currentTurn = 1;
	
	void StartSimulation ();
	void StopSimulation ();
	void EnterMiniGame ();
	void ExitMiniGame ();
	void StartNewTurn ();

	//Inputs
	void OnSpacePress ();

	//Temp
	bool _miniGameActive = false;

	//Prefabs
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _nuclearReactorPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _windmillPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _oilRigPrefab;

	//Materials
	UPROPERTY (EditAnywhere)
	UMaterialInterface* normalMaterial;
	UPROPERTY (EditAnywhere)
	UMaterialInterface* collideMaterial;
	
	//class UTextRenderComponent* CountdownText;
};
