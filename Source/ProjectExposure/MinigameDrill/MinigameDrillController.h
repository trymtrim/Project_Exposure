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

	int GetCurrentDrillType ();

	//For blueprints
	UPROPERTY (BlueprintReadOnly)
	bool showDrillUI = false;
	UPROPERTY (BlueprintReadOnly)
	FString livesText = "Lives: 3";
	UPROPERTY (BlueprintReadOnly)
	FString scoreText = "Score: 0";

	UFUNCTION (BlueprintImplementableEvent, Category = "Update")
	void Update ();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void EndGame ();
	void UpdateGameState (float deltaTime);
	void UpdateObstacles (float deltaTime);
	void SpawnObstacle ();
	void MovePlane (float deltaTime);
	void SetLives (int lives);
	void SetScore (int score);

	UFUNCTION (BLueprintCallable)
	void ChangeDrill (int index);

	ASimulationGameController* _gameController;

	AActor* _drill;
	AActor* _hole;
	AActor* _planeOne;
	AActor* _planeTwo;

	float _spawnInterval = 1.0f;
	float _timer = 0.0f;

	//Game stats
	int _lives = 3;
	int _score = 0;
	int _currentDrillType = 1;
	int _spawnCount = 0;
	float _endTimer = 0.0f;
	float _height = 2200.0f;
	bool _gameFinished = false;

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
