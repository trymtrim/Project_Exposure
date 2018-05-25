//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlaceableUnit.h"
#include "CameraMovement.h"
#include "MinigameCartController.h"
#include "MinigameDrill/MinigameDrillController.h"
#include "SimulationGameController.generated.h"

UCLASS()
class PROJECTEXPOSURE_API ASimulationGameController : public APawn
{
	GENERATED_BODY ()

public:
	ASimulationGameController ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	void ExitMiniGame ();

	//For blueprints
	UFUNCTION (BlueprintCallable, Category = "ButtonPress")
	void SpawnUnit (int index);

	UPROPERTY (BlueprintReadWrite)
	bool showUI = true;
	UPROPERTY (BlueprintReadWrite)
	bool showSimulationUI = false;

	UPROPERTY (BlueprintReadWrite)
	FString currentTurnText = "Turn 1";

protected:
	virtual void BeginPlay () override;

private:
	void PlaceUnit ();
	void StartSimulation ();
	void StopSimulation ();
	void EnterMiniGame ();
	void StartNewTurn ();
	void FadeIn (float delayTime, float fadeTime);
	void FadeOut (float delayTime, float fadeTime);
	void UpdateFading (float deltaTime);

	float fadeInTimer;
	float fadeOutTimer;
	float fadeInDelay;
	float fadeOutDelay;
	bool fadingIn = false;
	bool fadingOut = false;
	
	//Default camera position and rotation
	FVector _defaultPosition;
	FVector _defaultRotation;

	CameraMovement* _cameraMovement;

	//Currently controlled unit
	APlaceableUnit* _controlledUnit = nullptr;
	
	//Simulation setup
	int _maxTurns = 9;
	int _currentTurn = 1;

	//Inputs
	void OnSpacePress ();

	int _miniGameActive = 0; //0 = nothing, 1 = nuclear, 2 = windmill, 3 = oilrig
	bool _miniGameIsActive = false;

	//Temp
	int _firstPlayed = 0;

	//Prefabs
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _nuclearReactorPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _windmillPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _oilRigPrefab;

	//Minigame pawns, position and rotation
	//Mine game
	UPROPERTY (EditAnywhere)
	AMinigameCartController* _minePawn;
	UPROPERTY (EditAnywhere)
	FVector _minePosition;
	UPROPERTY (EditAnywhere)
	FVector _mineRotation;

	//Drill game
	UPROPERTY (EditAnywhere)
	AMinigameDrillController* _drillPawn;
	UPROPERTY (EditAnywhere)
	FVector _drillPosition;
	UPROPERTY (EditAnywhere)
	FVector _drillRotation;

	//Windmill game
	//
	//
	UPROPERTY (EditAnywhere)
	FVector _windmillPosition;
	UPROPERTY (EditAnywhere)
	FVector _windmillRotation;
};
