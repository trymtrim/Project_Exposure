// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameCartController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "SimulationGameController.h"
#include "IntVector.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "TimerManager.h"
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
	SetActorTickEnabled (false);

	UWorld* world = GetWorld();
	if (world) {
		FVector Location = GetActorLocation() + _offset;
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		_spawnedWagon = world->SpawnActor<AMinecart>(wagonPrefab, Location, _initTruckRotator, SpawnInfo);
		_spawnedWagon->setController(this);
	}
}

//Setup the minigame
void AMinigameCartController::setup() {

	SetActorTickEnabled (true);

	_uiController->Enable (12, 0);
	_lastMousePos = FVector2D::ZeroVector;
	_gameFinished = false;
}

void AMinigameCartController::startMinigame () {
	_uiController->Disable (12);

	_lives = _initialLives;
	_points = 0;
	scoreUI = "Score: 0";
	livesUI = "Life(s): 3";
	_uraniumLeftToSpawn = _uraniumToSpawn;
	_debrisLeftToSpawn = _debrisToSpawn;
	_nextSpawnDelay = FMath::RandRange(_spawnIntervalls.X, _spawnIntervalls.Y);
	//_maximumMovement = FVector2D(-400.0f, 400.0f);
	_uiController->Enable (6, 0);
	updateUI ();

	_gameStarted = true;
}

void AMinigameCartController::addPoints() {
	_points++;
	scoreUI = "Score: " + FString::FromInt(_points);
	updateUI ();
}

void AMinigameCartController::decreaseLives() {
	_lives--;
	livesUI = "Life(s): " + FString::FromInt(_lives);
	updateUI ();
	if (_lives == 0) {
		exitMinigame();
	}
}

//Called when minigames is finished by any means, handles cleaning up the minigame
void AMinigameCartController::exitMinigame() {
	if (_points < (int) _uraniumToSpawn / 3)
	{
		starAmount = 1;
		simulationController->SetMinigamePerformance (simulationController->BAD);
	}
	else if (_points > (int) _uraniumToSpawn / 3 && _points < (int) _uraniumToSpawn - ((int) _uraniumToSpawn / 3))
	{
		starAmount = 2;
		simulationController->SetMinigamePerformance (simulationController->NORMAL);
	}
	else 
	{
		starAmount = 3;
		simulationController->SetMinigamePerformance (simulationController->GOOD);
	}

	endScoreUI = FString::FromInt (_points) + "/" + FString::FromInt (_uraniumToSpawn);

	if (_lives <= 0)
		_uiController->Enable (11, 0); //Lose
	else
		_uiController->Enable (11, 0); //Win

	endScreen ();

	_gameStarted = false;
	_gameFinished = true;
	simulationController->StartClickDelay ();

	_uiController->Disable(6);

	SetActorTickEnabled (false);
}

void AMinigameCartController::goBackToSimulation() {
	FVector Location = GetActorLocation() + _offset;
	_spawnedWagon->SetActorLocationAndRotation(Location, _initTruckRotator);

	simulationController->ExitMiniGame();

	if (_lives <= 0)
		_uiController->Disable (11); //Lose
	else
		_uiController->Disable (11); //Win
}

// Called every frame
void AMinigameCartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_gameStarted)
	{
		movement(DeltaTime);
		spawnFallingUnit(DeltaTime);
	}

	if (_timerActive)
	{
		_timer += DeltaTime;

		if (_timer > 3.0f)
		{
			_timer = 0.0f;
			_timerActive = false;
			exitMinigame();
		}
	}
}

//Spawn a Falling Unit within the bounds the player can move in
void AMinigameCartController::spawnFallingUnit(float DeltaTime) {
	_nextSpawnDelay -= DeltaTime;

	if (_nextSpawnDelay <= 0.0f) {

		UWorld* world = GetWorld();
		if (world) {

			uint8 typeRand = FMath::FRand() * (_uraniumToSpawn + _debrisToSpawn);
			UnitType type;

			//I know that this is horrible, but I was under time pressure :'(
			//I MAY rework this later into something more pleasing
			if (typeRand <= _debrisToSpawn) {
				if (_debrisLeftToSpawn > 0) {
					type = UnitType::DEBRIS;
					_debrisLeftToSpawn--;
				}
				else if (_uraniumLeftToSpawn > 1) {
					type = UnitType::URANIUM;
					_uraniumLeftToSpawn--;
				}
				else {
					if(_uraniumLeftToSpawn > 0) {
						type = UnitType::URANIUM;
						_uraniumLeftToSpawn--;
					}
					else {
						_timerActive = true;
						return;
					}
				}
			} else {
				if (_uraniumLeftToSpawn > 1) {
					type = UnitType::URANIUM;
					_uraniumLeftToSpawn--;
				}
				else if(_debrisLeftToSpawn > 0) {
					type = UnitType::DEBRIS;
					_debrisLeftToSpawn--;
				}
				else {
					if(_uraniumLeftToSpawn > 0) {
						type = UnitType::URANIUM;
						_uraniumLeftToSpawn--;
					}
					else {
						_timerActive = true;
						return;
					}
				}
			}

			FVector Location = GetActorLocation() + FVector(_offset.X, FMath::RandRange(_maximumMovement.X, _maximumMovement.Y), 1000.0f);

			//Random Rotation for extra flair
			FRotator Rotation = FRotator::MakeFromEuler(FVector(0.0f, 180.0f, FMath::FRandRange(0.0f, 360.0f)));

			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AFallingUnit* unit = world->SpawnActor<AFallingUnit>(fallingUnitPrefab, Location, Rotation, SpawnInfo);
			//Passing info to the fallingUnit
			unit->init(type, _fallingUnitSpeed, _fallingUnitDeathThreshhold);
		}

		_nextSpawnDelay = FMath::RandRange(_spawnIntervalls.X, _spawnIntervalls.Y);
	}
}

void AMinigameCartController::movement(float DeltaTime) {
	//print(_isMoving ? "True" : "False");
	
	//Moving
	if (_isMoving) {
		//Handle the input
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		FVector2D mousePos;
		playerController->GetMousePosition(mousePos.X, mousePos.Y);

		FIntVector viewportBounds;
		playerController->GetViewportSize(viewportBounds.X, viewportBounds.Y);

		float percentage = mousePos.X / viewportBounds.X * 100;

		FVector wagonPosition = _spawnedWagon->GetActorLocation();
		FVector controllerLocation = GetActorLocation();
		float desiredLocation = controllerLocation.Y + FMath::Lerp(_maximumMovement.Y, _maximumMovement.X, percentage / 100);
		FVector minPosition = controllerLocation + FVector(0, _maximumMovement.X, 0);
		FVector maxPosition = controllerLocation + FVector(0, _maximumMovement.Y, 0);
		
		if (wagonPosition.Y > desiredLocation) {
			_faceLeft = true;
			_reachedRotation = false;
		} else if (wagonPosition.Y < desiredLocation) {
			_faceLeft = false;
			_reachedRotation = false;
		} else {
			//Exactly in the middle of viewport
		}

		//Do the movement
		if (wagonPosition.Y != desiredLocation && IsValid(_spawnedWagon)) {
			if (wagonPosition.Y < minPosition.Y)  {
			
			} else if (wagonPosition.Y > maxPosition.Y) {
				
			} else {
				FVector	newLocation = FMath::Lerp(wagonPosition ,FVector(wagonPosition.X, desiredLocation, wagonPosition.Z), DeltaTime * _speed);
				_spawnedWagon->SetActorLocation(newLocation);
			}
		}

		_lastMousePos = mousePos;
	}

	//Rotating
	if (!_reachedRotation) {
		FTransform wT = _spawnedWagon->GetActorTransform();
		FRotator wagonRotation = wT.GetRotation().Rotator();

		FRotator newRotation;
		if (_faceLeft) {
			newRotation = FMath::RInterpTo(wagonRotation, _leftRotator, DeltaTime, 7.5f);
		} else {
			newRotation = FMath::RInterpTo(wagonRotation, _rightRotator, DeltaTime, 7.5f);
		}

		wT.SetRotation(newRotation.Quaternion());
		_spawnedWagon->SetActorTransform(wT);
	}
}

//Handle the input we get from the pawn
void AMinigameCartController::handleAxisInput(float axisValue) {
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

void AMinigameCartController::handleTouchInputStart() {
	_isMoving = true;
}

void AMinigameCartController::handleTouchInputEnd() {
	_isMoving = false;
	_velocity.Y = 0.0f;
}

void AMinigameCartController::onMouseClick ()
{
	if (_gameStarted)
		handleTouchInputStart ();
	else if (_gameFinished)
	{
		if (simulationController->CanContinue ())
			goBackToSimulation();
	}
	else
		startMinigame ();
}

// Called to bind functionality to input
void AMinigameCartController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis("HorizontalAxis", this, &AMinigameCartController::handleAxisInput);
	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AMinigameCartController::onMouseClick);
	PlayerInputComponent->BindAction("MouseClick", IE_Released, this, &AMinigameCartController::handleTouchInputEnd);
}

