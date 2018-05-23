// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameCartController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "SimulationGameController.h"
#include "MinigameCart/FallingUnit.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
AMinigameCartController::AMinigameCartController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMinigameCartController::BeginPlay()
{
	Super::BeginPlay();
	setup();
}

//Setup the minigame, especially the timers
void AMinigameCartController::setup() {

	UWorld* world = GetWorld();
	if (world) {
		FVector Location(0, 0, 0);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		_spawnedWagon = world->SpawnActor<AMinecart>(wagonPrefab, Location, Rotation, SpawnInfo);
		_spawnedWagon->setController(this);
		print("Spawned Wagon");
	}

	_lives = _initialLives;

	FTimerHandle durationTimer;
	GetWorldTimerManager().SetTimer(durationTimer, this, &AMinigameCartController::exitMinigame, _minigameDuration, false);

	FTimerHandle spawnTimer;
	GetWorldTimerManager().SetTimer(spawnTimer, this, &AMinigameCartController::spawnFallingUnit, 5.0f, true, 2.0f);
}

void AMinigameCartController::addPoints() {
	_points++;
}

void AMinigameCartController::decreaseLives() {
	_lives--;

	if (_lives == 0) {
		exitMinigame();
	}
}

//Called when minigames is finished by any means, handles cleaning up the minigame
void AMinigameCartController::exitMinigame() {
	
	for (TActorIterator<AFallingUnit> Itr (GetWorld()); Itr; ++Itr) {
		Itr->Destroy();
	}

	_spawnedWagon->Destroy();
	
	simulationController->ExitMiniGame();
}

// Called every frame
void AMinigameCartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Moving
	if (!_velocity.IsZero() && IsValid(_spawnedWagon)) {
		FVector NewLocation = _spawnedWagon->GetActorLocation() + (_velocity * _speed * DeltaTime);

		if (NewLocation.Y < _maximumMovement.X || NewLocation.Y > _maximumMovement.Y) {
			//TODO: handle out of bounds
		}
		else _spawnedWagon->SetActorLocation(NewLocation);
	}

	//Rotating
	if (!_reachedRotation) {
		FTransform wT = _spawnedWagon->GetActorTransform();
		FRotator wagonRotation = wT.GetRotation().Rotator();

		FRotator newRotation;
		if (_faceLeft) {
			newRotation = FMath::RInterpTo(wagonRotation, _leftRotator, DeltaTime, 7.5f);
		}
		else {
			newRotation = FMath::RInterpTo(wagonRotation, _rightRotator, DeltaTime, 7.5f);
		}

		wT.SetRotation(newRotation.Quaternion());
		_spawnedWagon->SetActorTransform(wT);
	}
}

//Spawn a Falling Unit within the bounds the player can move in
void AMinigameCartController::spawnFallingUnit() {
	UWorld* world = GetWorld();
	if (world) {
		FVector Location(0, FMath::RandRange(_maximumMovement.X, _maximumMovement.Y), 1000.0f);
		//Random Rotation for extra flair
		FRotator Rotation(0.0f, 0.0f, FMath::FRandRange(0.0f, 360.0f));
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AFallingUnit* unit = world->SpawnActor<AFallingUnit>(fallingUnitPrefab, Location, Rotation, SpawnInfo);
		//Passing info to the fallingUnit
		unit->init(_fallingUnitSpeed, _fallingUnitDeathThreshhold);
	}
}

//Handle the input we get from the pawn
void AMinigameCartController::handleInput(float axisValue) {
	_velocity.Y = FMath::Clamp(axisValue, -1.0f, 1.0f) * 100.0f;

	//Interpret the input to a rotation 
	if (_velocity.Y < 0.0f) {
		_faceLeft = true;
		_reachedRotation = false;
	}
	else if (_velocity.Y > 0.0f) {
		_faceLeft = false;
		_reachedRotation = false;
	}
}

// Called to bind functionality to input
void AMinigameCartController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("HorizontalAxis", this, &AMinigameCartController::handleInput);
	//PlayerInputComponent->BindAction ("Space", IE_Pressed, this, &AMinigameCartController::setup);
}

