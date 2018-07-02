//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MinigameSolarController.generated.h"

class ASimulationGameController;
class AUIController;

UCLASS()
class PROJECTEXPOSURE_API AMinigameSolarController : public APawn
{
	GENERATED_BODY ()

public:
	//Sets default values for this pawn's properties
	AMinigameSolarController ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	void SetGameController (ASimulationGameController* gameController);
	void StartGame ();

	//For blueprints
	UPROPERTY (BlueprintReadOnly)
	FString scoreText = "0";
	UPROPERTY (BlueprintReadOnly)
	FString endScoreText = "";

	UFUNCTION (BlueprintImplementableEvent, Category = "SolarUpdate")
	void Update ();
	UFUNCTION (BlueprintImplementableEvent, Category = "SolarEnd")
	void EndScreen ();
	UFUNCTION (BlueprintImplementableEvent, Category = "EndSolar")
	void OneStar ();
	UFUNCTION (BlueprintImplementableEvent, Category = "EndSolar")
	void ThreeStars ();
	UFUNCTION (BlueprintImplementableEvent, Category = "EndSolar")
	void TwoStar ();

	UPROPERTY (BlueprintReadOnly)
	float _barValue = 0.0f;
	UPROPERTY (BlueprintReadOnly)
	FString timerText = "01:00";

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void InitializeGame ();
	void InitializeGrid ();
	void ControlUnit ();
	void PlaceUnit ();
	void RotateUnit (AActor* mirror);
	void UpdateBeams ();
	void CheckBeamHit (FVector position, FRotator rotation, bool firstBeam);
	void StartGamePlay ();
	void EndGame ();
	void SetScore (int score);
	void GoBackToSimulation ();
	void OnMouseClick ();
	void OnMouseRelease ();
	void AnimateStars (float deltaTime);
	void AddObstacle (int gridIndex);

	bool _animatingStars = false;
	float _starLerp = 0.0f;
	int _currentScoreLerp = 0;

	AActor* _controlledUnit = nullptr;
	bool _lifting = false;

	ASimulationGameController* _gameController;

	//Game positions
	UPROPERTY () FVector _middlePosition = FVector (7955, 6612, 110);
	UPROPERTY () AActor* _beamPosition;
	UPROPERTY () AActor* _goalPosition;
	UPROPERTY () AActor* _goalTarget;
	UPROPERTY () TArray <AActor*> _grid;
	UPROPERTY () TArray <AActor*> _mirrors;
	UPROPERTY () TMap <AActor*, int> _mirrorRotations;
	UPROPERTY () TArray <AActor*> _beams;
	UPROPERTY () AActor* _startBeam;
	UPROPERTY () TArray <AActor*> _rotators;

	//Game stats
	int _score = 0;
	int _maxScore = 30;

	bool _gameStarted = false;
	bool _endPanelShown = false;

	float _timer = 60.0f;

	UPROPERTY () TArray <AActor*> _obstacles;

	//Prefabs
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _gridSlot;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _mirror;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _beam;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _goal;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _goalTargetPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _rotatePrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _obstaclePrefab;

	//UI controller
	UPROPERTY (EditAnywhere)
	AUIController* _uiController;

	//Game setup
	UPROPERTY (EditAnywhere)
	int _mirrorCount;
	UPROPERTY (EditAnywhere)
	int _gridX;
	UPROPERTY (EditAnywhere)
	int _gridY;
	UPROPERTY (EditAnywhere)
	int _beamStartRotation;
	UPROPERTY (EditAnywhere)
	int _beamStartPositionX;
	UPROPERTY (EditAnywhere)
	int _beamStartPositionY;
	UPROPERTY (EditAnywhere)
	int _beamGoalPositionX;
	UPROPERTY (EditAnywhere)
	int _beamGoalPositionY;
};
