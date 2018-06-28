// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "MinigameCart/Minecart.h"
#include "UIController.h"
#include "Particles/Emitter.h"
#include "MinigameCartController.generated.h"

class ASimulationGameController;

UCLASS()
class PROJECTEXPOSURE_API AMinigameCartController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinigameCartController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Methods the Minecart can call
	void addPoints();
	void decreaseLives();
	void setup();

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void updateUI();
	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void endScreen();
	UFUNCTION (BlueprintImplementableEvent, Category = "EndCart")
	void OneStar ();
	UFUNCTION (BlueprintImplementableEvent, Category = "EndCart")
	void ThreeStars ();
	UFUNCTION (BlueprintImplementableEvent, Category = "EndCart")
	void TwoStar ();

	UPROPERTY (BlueprintReadOnly)
	float _barValue = 0.0f;

	UPROPERTY (BlueprintReadOnly)
	FString endScoreUI = "";
	UPROPERTY (BlueprintReadOnly)
	int starAmount = 1;

	UPROPERTY(BlueprintReadOnly)
	FString livesUI = "Life(s): 3";

	UPROPERTY(BlueprintReadOnly)
	FString scoreUI = "Score: 0";

	//Prefabs and references
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> wagonPrefab;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> fallingUnitPrefab;

	UPROPERTY(EditAnywhere)
	ASimulationGameController* simulationController;

private:

	void handleAxisInput(float axisValue);
	void handleTouchInputStart();
	void handleTouchInputEnd();
	void movement(float DeltaTime);

	void spawnFallingUnit(float DeltaTime);
	void exitMinigame();
	void startMinigame();
	void goBackToSimulation();
	void AnimateStars (float deltaTime);

	void onMouseClick();

	bool _animatingStars = false;
	float _starLerp = 0.0f;
	int _currentScoreLerp = 0;

	UPROPERTY () 
	AMinecart* _spawnedWagon;
	UPROPERTY()
	FVector _velocity;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "1000.0", UIMin = "1.0", UIMax = "1000.0"))
	uint32 _uraniumToSpawn;
	UPROPERTY(VisibleAnywhere)
	uint32 _uraniumLeftToSpawn;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "1000.0", UIMin = "1.0", UIMax = "1000.0"))
	uint32 _debrisToSpawn;
	UPROPERTY(VisibleAnywhere)
	uint32 _debrisLeftToSpawn;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.01", ClampMax = "3.0", UIMin = "0.01", UIMax = "3.0"))
	FVector2D _spawnIntervalls;
	float _nextSpawnDelay;

	UPROPERTY(EditAnywhere)
	float _speed;
	
	UPROPERTY()
	bool _isMoving = false;

	UPROPERTY(EditAnywhere)
	FVector2D _maximumMovement;

	UPROPERTY(EditAnywhere)
	uint8 _initialLives;

	UPROPERTY(EditAnywhere)
	float _fallingUnitSpeed;

	UPROPERTY(EditAnywhere)
	float _fallingUnitDeathThreshhold;

	UPROPERTY(VisibleAnywhere)
	int _points = 0;

	UPROPERTY(VisibleAnywhere)
	uint8 _lives;

	UPROPERTY()
	FVector2D _lastMousePos;

	UPROPERTY(EditAnywhere)
	FVector _offset = FVector(-500, 0, -375);

	//Rotation Stuff
	bool _faceLeft;
	bool _reachedRotation;
	
	bool _gameStarted = false;
	bool _gameFinished = false;

	UPROPERTY()
	const FRotator _leftRotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 0.0f));
	UPROPERTY()
	const FRotator _rightRotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 180.0f));
	UPROPERTY()
	const FRotator _initTruckRotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 180.0f));

	bool _timerActive = false;
	float _timer = 0.0f;

	UPROPERTY(EditAnywhere)
	AUIController* _uiController;

	UPROPERTY(EditAnywhere)
	AEmitter* _diggerEmitter;
};
