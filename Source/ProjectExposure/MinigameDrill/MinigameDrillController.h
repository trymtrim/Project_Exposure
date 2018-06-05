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

	int GetCurrentDrillType ();

	//For blueprints
	UPROPERTY (BlueprintReadWrite)
	bool showDrillUI = false;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void EndGame ();
	void UpdateGameState (float deltaTime);
	void UpdateObstacles (float deltaTime);
	void SpawnObstacle ();
	void MovePlane (float deltaTime);

	UFUNCTION (BLueprintCallable)
	void ChangeDrill (int index);

	ASimulationGameController* _gameController;

	AActor* _drill;
	AActor* _planeOne;
	AActor* _planeTwo;

	float _spawnInterval = 1.0f;
	float _timer = 0.0f;

	//Game stats
	int _currentDrillType = 1;
	int _health = 3;
	float _gameTimer = 0.0f;
	float _height = -1800.0f;

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
	TArray <TSubclassOf <AActor>> _drillPrefabs;
	UPROPERTY (EditAnywhere)
	TArray <TSubclassOf <AActor>> _obstaclePrefabs;
};
