// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "MinigameCart/Minecart.h"
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
	
	//Prefabs and references
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> wagonPrefab;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> fallingUnitPrefab;

	UPROPERTY(EditAnywhere)
	ASimulationGameController* simulationController;

private:

	void setup();
	void handleInput(float axisValue);

	void spawnFallingUnit();
	void exitMinigame();

	UPROPERTY()
	AMinecart* _spawnedWagon;

	UPROPERTY()
	FVector _velocity;

	UPROPERTY(EditAnywhere)
	uint8 _minigameDuration;

	UPROPERTY(EditAnywhere)
	float _speed;

	UPROPERTY(EditAnywhere)
	FVector2D _maximumMovement;

	UPROPERTY(EditAnywhere)
	uint8 _initialLives;

	UPROPERTY(EditAnywhere)
	float _fallingUnitSpeed;

	UPROPERTY(EditAnywhere)
	float _fallingUnitDeathThreshhold;

	UPROPERTY(VisibleAnywhere)
	int _points;

	UPROPERTY(VisibleAnywhere)
	uint8 _lives;
	
	//Rotation Stuff
	UPROPERTY()
	bool _faceLeft;

	UPROPERTY()
	bool _reachedRotation;

	const FRotator _leftRotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 0.0f));
	const FRotator _rightRotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 180.0f));
};
