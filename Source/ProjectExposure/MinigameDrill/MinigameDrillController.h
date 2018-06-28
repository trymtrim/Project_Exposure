//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MinigameDrillController.generated.h"

class ASimulationGameController;
class ADrillObstacle;
class AUIController;

UCLASS()
class PROJECTEXPOSURE_API AMinigameDrillController : public APawn
{
	GENERATED_BODY ()

public:
	//Sets default values for this pawn's properties
	AMinigameDrillController ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	void SetGameController (ASimulationGameController* gameController);
	void StartGame ();
	void GetHitByObstacle ();
	void OvercomeObstacle ();

	bool GetGameFinished ();
	bool GetGameCompleted ();
	int GetCurrentDrillType ();
	int GetSpawnCount ();

	//For blueprints
	UPROPERTY (BlueprintReadOnly)
	FString livesText = "3";
	UPROPERTY (BlueprintReadOnly)
	FString scoreText = "0";
	UPROPERTY (BlueprintReadOnly)
	FString endScoreText = "";
	UPROPERTY (BlueprintReadOnly)
	int starAmount = 1;
	UPROPERTY (BlueprintReadOnly)
	float _barValue = 0.0f;

	UFUNCTION (BlueprintImplementableEvent, Category = "Update")
	void Update ();
	UFUNCTION (BlueprintImplementableEvent, Category = "Start")
	void Start ();
	UFUNCTION (BlueprintImplementableEvent, Category = "Stop")
	void Stop ();
	UFUNCTION (BlueprintImplementableEvent, Category = "End")
	void EndScreen ();
	UFUNCTION (BlueprintImplementableEvent, Category = "End")
	void OneStar ();
	UFUNCTION (BlueprintImplementableEvent, Category = "End")
	void ThreeStars ();
	UFUNCTION (BlueprintImplementableEvent, Category = "End")
	void TwoStar ();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void StartGamePlay ();
	void StartDisableUI ();
	void EndGame ();
	void GoBackToSimulation ();
	void UpdateGameState (float deltaTime);
	void UpdateObstacles (float deltaTime);
	void SpawnObstacle ();
	void MovePlane (float deltaTime);
	void SetLives (int lives);
	void SetScore (int score);
	void OnMouseClick ();
	void AnimateStars (float deltaTime);

	UFUNCTION (BLueprintCallable)
	void ChangeDrill (int index);

	ASimulationGameController* _gameController;

	UPROPERTY () AActor* _drill;
	UPROPERTY () AActor* _hole;
	UPROPERTY () AActor* _planeOne;
	UPROPERTY () AActor* _planeTwo;

	float _spawnInterval = 1.0f;
	float _timer = 0.0f;

	float _disableUITimer = 0.0f;
	bool _disableUI = false;

	bool _gameStarted = false;

	bool _animatingStars = false;
	float _starLerp = 0.0f;
	int _currentScoreLerp = 0;

	//Game stats
	int _lives = 3;
	int _score = 0;
	int _currentDrillType = 1;
	int _spawnCount = 0;
	float _endTimer = 0.0f;
	float _height = 2200.0f;
	bool _gameFinished = false;
	bool _gameCompleted = false;
	bool _endPanelShown = false;

	//Spawn info
	FActorSpawnParameters spawnParams;
	FVector spawnPosition;
	FRotator rotator;

	//UI controller
	UPROPERTY (EditAnywhere)
	AUIController* _uiController;

	//Prefabs
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _planePrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _holePrefab;
	UPROPERTY (EditAnywhere)
	TArray <TSubclassOf <AActor>> _drillPrefabs;
	UPROPERTY (EditAnywhere)
	TArray <TSubclassOf <AActor>> _obstaclePrefabs;
};
