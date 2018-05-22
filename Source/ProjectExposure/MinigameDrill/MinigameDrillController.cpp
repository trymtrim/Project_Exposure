//Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameDrillController.h"
#include "SimulationGameController.h"
#include "Drill.h"
#include "DrillObstacle.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
AMinigameDrillController::AMinigameDrillController ()
{
 	//Set this pawn to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void AMinigameDrillController::BeginPlay ()
{
	Super::BeginPlay ();

	SetActorTickEnabled (false);
}

//Called every frame
void AMinigameDrillController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	//Will only run while game is running
	UpdateGameState (DeltaTime);
	UpdateObstacles (DeltaTime);
	MovePlane ();
}

void AMinigameDrillController::SetGameController (ASimulationGameController* gameController)
{
	_gameController = gameController;
}

void AMinigameDrillController::StartGame ()
{
	SetActorTickEnabled (true);

	_gameTimer = 0.0f;
	_health = 3;
	_currentDrillType = 1;

	FActorSpawnParameters spawnParams;

	FVector spawnPosition = FVector (3550.0f, -6150.0f, 700.0f);
	FRotator rotator = FRotator::MakeFromEuler (FVector (0.0f, 0.0f, 0.0f));
	
	//Spawn drill
	_drill = GetWorld ()->SpawnActor <ADrill> (_drillPrefab, spawnPosition, rotator, spawnParams);

	FRotator planeRotator = FRotator::MakeFromEuler (FVector (270.0f, 0.0f, 0.0f));
	FVector firstPlaneSpawnPosition = spawnPosition - FVector (0.0f, 0.0f, 500.0f);

	//Spawn planes
	_planeOne = GetWorld ()->SpawnActor <AActor> (_planePrefab, firstPlaneSpawnPosition, planeRotator, spawnParams);
	_planeOne->SetActorScale3D (FVector (10.0f, 10.0f, 1.0f));

	FVector secondPlaneSpawnPosition = spawnPosition - FVector (0.0f, 0.0f, -500.0f);

	_planeTwo = GetWorld ()->SpawnActor <AActor> (_planePrefab, secondPlaneSpawnPosition, planeRotator, spawnParams);
	_planeTwo->SetActorScale3D (FVector (10.0f, 10.0f, 1.0f));
}

void AMinigameDrillController::EndGame ()
{
	SetActorTickEnabled (false);

	_drill->Destroy ();
	_planeOne->Destroy ();
	_planeTwo->Destroy ();

	_gameController->ExitMiniGame ();
}

void AMinigameDrillController::UpdateGameState (float deltaTime)
{
	_gameTimer += deltaTime;

	if (_gameTimer >= 6.0f)
		EndGame ();
}

void AMinigameDrillController::UpdateObstacles (float deltaTime)
{
	_timer += deltaTime;

	if (_timer >= _spawnInterval)
	{
		SpawnObstacle ();
		_timer = 0.0f;
	}
}

void AMinigameDrillController::SpawnObstacle ()
{
	FActorSpawnParameters spawnParams;
		
	FVector spawnPosition = FVector (_drill->GetActorLocation ().X, _drill->GetActorLocation ().Y, 0.0f);
	FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

	//Spawn random obstacle from array of obstacles
	TSubclassOf <AActor> obstacle = _obstaclePrefabs [FMath::RandRange (0, _obstaclePrefabs.Num () - 1)];
	ADrillObstacle* spawnedObstacle = GetWorld ()->SpawnActor <ADrillObstacle> (obstacle, spawnPosition, rotator, spawnParams);

	spawnedObstacle->Initialize (_drill, this);
}

void AMinigameDrillController::ChangeDrill (int index)
{
	_currentDrillType = index;
}

void AMinigameDrillController::MovePlane ()
{
	_planeOne->SetActorLocation (_planeOne->GetActorLocation () + FVector (0.0f, 0.0f, 5.0f));
	_planeTwo->SetActorLocation (_planeTwo->GetActorLocation () + FVector (0.0f, 0.0f, 5.0f));

	if (_planeOne->GetActorLocation ().Z > 1500.0f)
		_planeOne->SetActorLocation (_planeTwo->GetActorLocation () - FVector (0.0f, 0.0f, 1000.0f));
	if (_planeTwo->GetActorLocation ().Z > 1500.0f)
		_planeTwo->SetActorLocation (_planeOne->GetActorLocation () - FVector (0.0f, 0.0f, 1000.0f));
}

void AMinigameDrillController::GetHitByObstacle ()
{
	_health--;

	if (_health == 0)
	{
		EndGame ();
	}
}

int AMinigameDrillController::GetCurrentDrillType ()
{
	return _currentDrillType;
}

//Called to bind functionality to input
void AMinigameDrillController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);
}
