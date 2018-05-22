//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MinigameDrillController.generated.h"

class ASimulationGameController;
class ADrill;
class ADrillObstacle;

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

	int GetCurrentDrillType ();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void EndGame ();
	void UpdateGameState (float deltaTime);
	void UpdateObstacles (float deltaTime);
	void SpawnObstacle ();
	void ChangeDrill (int index);
	void MovePlane ();

	ASimulationGameController* _gameController;

	ADrill* _drill;
	AActor* _planeOne;
	AActor* _planeTwo;

	float _spawnInterval = 2.0f;
	float _timer = 0.0f;

	//Game stats
	int _currentDrillType = 1;
	int _health = 3;
	float _gameTimer = 0.0f;

	//Prefabs
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _drillPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _planePrefab;
	UPROPERTY (EditAnywhere)
	TArray <TSubclassOf <AActor>> _obstaclePrefabs;
};
