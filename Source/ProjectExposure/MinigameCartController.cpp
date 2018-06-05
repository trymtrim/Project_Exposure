// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameCartController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "SimulationGameController.h"
#include "IntVector.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
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
		FVector Location = GetActorLocation() + FVector(-500, 0, -375);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		_spawnedWagon = world->SpawnActor<AMinecart>(wagonPrefab, Location, _initTruckRotator, SpawnInfo);
		_spawnedWagon->setController(this);
	}
}

//Setup the minigame
void AMinigameCartController::setup() {

	SetActorTickEnabled (true);

	_lives = _initialLives;
	_points = 0;
	scoreUI = "Score: 0";
	livesUI = "Life(s): 3";
	updateUI ();
	_uraniumLeftToSpawn = _uraniumToSpawn;
	_debrisLeftToSpawn = _debrisToSpawn;
	_nextSpawnDelay = FMath::RandRange(_spawnIntervalls.X, _spawnIntervalls.Y);
	//_maximumMovement = FVector2D(-400.0f, 400.0f);
	_uiController->Enable (_uiController->cartMiniGameRef, 0);
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
	FVector Location = GetActorLocation() + FVector(-500, 0, -375);
	_spawnedWagon->SetActorLocationAndRotation(Location, _initTruckRotator);

	simulationController->ExitMiniGame();
	_uiController->Disable(_uiController->cartMiniGameRef);
	SetActorTickEnabled (false);
}

// Called every frame
void AMinigameCartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	movement(DeltaTime);
	spawnFallingUnit(DeltaTime);
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
				else if (_uraniumLeftToSpawn > 0) {
					type = UnitType::URANIUM;
					_uraniumLeftToSpawn--;
				}
				else {
					exitMinigame();
					return;
				}
			} else {
				if (_uraniumLeftToSpawn > 0) {
					type = UnitType::URANIUM;
					_uraniumLeftToSpawn--;
				}
				else if(_debrisLeftToSpawn > 0) {
					type = UnitType::DEBRIS;
					_debrisLeftToSpawn--;
				}
				else {
					exitMinigame();
					return;
				}
			}

			FVector Location = GetActorLocation() + FVector(-500.0f, FMath::RandRange(_maximumMovement.X, _maximumMovement.Y), 1000.0f);

			//Random Rotation for extra flair
			FRotator Rotation(0.0f, 0.0f, FMath::FRandRange(0.0f, 360.0f));

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

		if (mousePos.X > viewportBounds.X / 2) {
			_velocity.Y = -100.0f;
			_faceLeft = true;
			_reachedRotation = false;
		} else if (mousePos.X < viewportBounds.X / 2) {
			_velocity.Y = 100.0f;
			_faceLeft = false;
			_reachedRotation = false;
		} else {
			//Exactly in the middle of viewport
		}

		//Do the movement
		if (!_velocity.IsZero() && IsValid(_spawnedWagon)) {
			FVector NewLocation = _spawnedWagon->GetActorLocation() + (_velocity * _speed * DeltaTime);

			FVector controllerLocation = GetActorLocation();
			//FString debug = "ControllerLocation: " + controllerLocation.ToString() + " NewLocation: " + NewLocation.ToString() + " MaxiumumMovement: " + _maximumMovement.ToString();
			//print(debug);
			if (NewLocation.Y < controllerLocation.Y + _maximumMovement.X)  {
			
			} else if (NewLocation.Y > controllerLocation.Y + _maximumMovement.Y) {
				
			} else {
				_spawnedWagon->SetActorLocation(NewLocation);
			}
		}
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

// Called to bind functionality to input
void AMinigameCartController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis("HorizontalAxis", this, &AMinigameCartController::handleAxisInput);
	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AMinigameCartController::handleTouchInputStart);
	PlayerInputComponent->BindAction("MouseClick", IE_Released, this, &AMinigameCartController::handleTouchInputEnd);
}

