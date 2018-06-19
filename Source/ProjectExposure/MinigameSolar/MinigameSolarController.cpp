//Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameSolarController.h"
#include "SimulationGameController.h"
#include "Engine/World.h"
#include "UIController.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
AMinigameSolarController::AMinigameSolarController ()
{
 	//Set this pawn to call Tick () every frame
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void AMinigameSolarController::BeginPlay ()
{
	Super::BeginPlay ();

	SetActorTickEnabled (false);
}

//Called every frame
void AMinigameSolarController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	if (_controlledUnit)
	{
		//Trace to see what is under the mouse cursor
		FHitResult hit;
		GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

		if (hit.bBlockingHit)
		{
			FVector position = FVector (hit.Location.X, hit.Location.Y, _middlePosition.Z);
			_controlledUnit->SetActorLocation (position);
		}
	}

	if (_preparingLifting)
	{
		_liftingTimer += DeltaTime;

		if (_liftingTimer >= 0.15f)
		{
			_preparingLifting = false;
			_liftingTimer = 0.0f;

			ControlUnit ();
		}
	}
}

void AMinigameSolarController::InitializeGame ()
{
	InitializeGrid ();

	UWorld* world = GetWorld ();

	if (world)
	{
		//Spawn mirrors
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _middlePosition + FVector (300.0f, 200.0f, 0.0f);
		FRotator rotator = FRotator (0.0f, 45.0f, 0.0f);

		for (int i = 0; i < _mirrorCount; i++)
		{
			AActor* mirror = world->SpawnActor <AActor> (_mirror, spawnPosition, rotator, spawnParams);
			_mirrors.Add (mirror);
			_mirrorRotations.Add (mirror, 1);

			spawnPosition.Y -= 100.0f;
		}

		//Spawn beam start position
		switch (_beamStartRotation)
		{
		case 1:
			rotator = FRotator (0.0f, 90.0f, 0.0f);
			break;
		case 2:
			rotator = FRotator (0.0f, -180.0f, 0.0f);
			break;
		case 3:
			rotator = FRotator (0.0f, -90.0f, 0.0f);
			break;
		case 4:
			rotator = FRotator (0.0f, 0.0f, 0.0f);
			break;
		}

		spawnPosition = _grid [((_gridX * _gridY) - 1) / 2]->GetActorLocation () + FVector (_beamStartPositionX * 100, _beamStartPositionY * 100, 0.0f);
		_beamPosition = world->SpawnActor <AActor> (_gridSlot, spawnPosition, rotator, spawnParams);
		//Spawn beam end position
		spawnPosition = _grid [((_gridX * _gridY) - 1) / 2]->GetActorLocation () + FVector (_beamGoalPositionX * 100, _beamGoalPositionY * 100, 0.0f);
		_goalPosition = world->SpawnActor <AActor> (_goal, spawnPosition, rotator, spawnParams);
		//Spawn beam goal target
		spawnPosition += FVector (0.0f, 0.0f, 120.0f);
		_goalTarget = world->SpawnActor <AActor> (_goalTargetPrefab, spawnPosition, rotator, spawnParams);
	}
}

void AMinigameSolarController::InitializeGrid ()
{
	UWorld* world = GetWorld ();

	int slotSize = 100;

	if (world)
	{
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _middlePosition - FVector ((slotSize * _gridX) / 2, (slotSize * _gridY) / 2, 0.0f);
		FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

		for (int i = 0; i < _gridX; i++)
		{
			for (int j = 0; j < _gridY; j++)
			{
				//Spawn the unit
				AActor* gridSlot = world->SpawnActor <AActor> (_gridSlot, spawnPosition, rotator, spawnParams);
				_grid.Add (gridSlot);

				spawnPosition.Y += slotSize;
			}

			spawnPosition.Y -= (slotSize * _gridY);
			spawnPosition.X += slotSize;
		}
	}
}

void AMinigameSolarController::ControlUnit ()
{
	//Trace to see what is under the mouse cursor
	FHitResult hit;
	GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

	if (hit.bBlockingHit)
	{
		if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Mirror"))
		{
			_controlledUnit = hit.GetActor ();
			_lifting = true;
		}
	}
}

void AMinigameSolarController::PlaceUnit ()
{
	AActor* closestGridSlot = nullptr;
		
	for (int i = 0; i < _grid.Num (); i++)
	{
		AActor* gridSlot = _grid [i];

		bool skip = false;

		for (int i = 0; i < _mirrors.Num (); i++)
		{
			if (FVector::Distance (_mirrors [i]->GetActorLocation (), gridSlot->GetActorLocation ()) < 5)
			{
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			if (!closestGridSlot)
				closestGridSlot = gridSlot;
			else if (FVector::Distance (gridSlot->GetActorLocation (), _controlledUnit->GetActorLocation ()) < FVector::Distance (closestGridSlot->GetActorLocation (), _controlledUnit->GetActorLocation ()))
				closestGridSlot = gridSlot;
		}
	}
		
	_controlledUnit->SetActorLocation (closestGridSlot->GetActorLocation ());

	_lifting = false;
	_controlledUnit = nullptr;

	UpdateBeams ();
}

void AMinigameSolarController::RotateUnit ()
{
	FHitResult hit;
	GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

	if (hit.bBlockingHit)
	{
		AActor* mirror = hit.GetActor ();

		if (mirror->GetRootComponent ()->ComponentHasTag ("Mirror"))
		{
			mirror->SetActorRotation (hit.GetActor ()->GetActorRotation () + FRotator (0.0f, 90.0f, 0.0f));

			if (_mirrorRotations [mirror] == 4)
				_mirrorRotations [mirror] = 1;
			else
				_mirrorRotations [mirror] = _mirrorRotations [mirror] + 1;

			UpdateBeams ();
		}
	}
}

void AMinigameSolarController::UpdateBeams ()
{
	//Remove all current beams
	for (int i = 0; i < _beams.Num (); i++)
		_beams [i]->Destroy ();

	_beams.Empty ();

	CheckBeamHit (_beamPosition->GetActorLocation (), _beamPosition->GetActorRotation (), true);
}

void AMinigameSolarController::CheckBeamHit (FVector position, FRotator rotation, bool firstBeam)
{
	//Spawn the beam
	FActorSpawnParameters spawnParams;
	FVector spawnPosition = position + FVector (0.0f, 00.0f, 50.0f);
	FRotator rotator = rotation;

	AActor* beam = GetWorld ()->SpawnActor <AActor> (_beam, spawnPosition, rotator, spawnParams);
	_beams.Add (beam);

	FHitResult hit;
	FVector start = beam->GetActorLocation ();

	if (firstBeam)
		_startBeam = beam;

	FVector forwardVector = beam->GetActorForwardVector ();
	FVector end = (forwardVector * 1000.0f) + start;
	FCollisionQueryParams collisionParams;

	DrawDebugLine (GetWorld (), start, end, FColor::Green, false, 1, 0, 10);

	TArray <AActor*> possibleMirrors;

	for (int i = 0; i < _mirrors.Num (); i++)
	{
		if (_mirrors [i]->ActorLineTraceSingle (hit, start + (forwardVector * 75), end, ECC_WorldDynamic, collisionParams))
			possibleMirrors.Add (_mirrors [i]);
	}

	if (possibleMirrors.Num () > 0)
	{
		int closestMirrorIndex = 0;

		for (int i = 1; i < possibleMirrors.Num (); i++)
		{
			if (FVector::Distance (start, possibleMirrors [i]->GetActorLocation ()) < FVector::Distance (start, possibleMirrors [closestMirrorIndex]->GetActorLocation ()))
				closestMirrorIndex = i;
		}

		AActor* hitMirror = possibleMirrors [closestMirrorIndex];

		//Get the beam particle component
		TArray <UParticleSystemComponent*> staticComps;
		beam->GetComponents <UParticleSystemComponent> (staticComps);
		UParticleSystemComponent* beamComponent = staticComps [0];

		//Set beam start and end position
		beamComponent->SetActorParameter ("Source", beam);
		beamComponent->SetActorParameter ("Target", hitMirror);

		//Mirror rotation
		int rot = rotation.Yaw;

		switch (_mirrorRotations [hitMirror])
		{
		case 1:
			if (rot < 5 && rot > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, -90.0f, 0.0f), false);
			else if (rot - 90 < 5 && rot - 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 180.0f, 0.0f), false);
			break;
		case 2:
			if (rot - 90 < 5 && rot - 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 0.0f, 0.0f), false);
			else if (rot - 180 < 5 && rot - 180 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, -90.0f, 0.0f), false);
			break;
		case 3:
			if (rot + 90 < 5 && rot + 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 0.0f, 0.0f), false);
			else if (rot - 180 < 5 && rot - 180 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 90.0f, 0.0f), false);
			break;
		case 4:
			if (rot < 5 && rot > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 90.0f, 0.0f), false);
			else if (rot + 90 < 5 && rot + 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 180.0f, 0.0f), false);
			break;
		}
	}
	else
	{
		if (_goalPosition->ActorLineTraceSingle (hit, start + (forwardVector * 75), end, ECC_WorldDynamic, collisionParams))
		{
			//Get the beam particle component
			TArray <UParticleSystemComponent*> staticComps;
			beam->GetComponents <UParticleSystemComponent> (staticComps);
			UParticleSystemComponent* beamComponent = staticComps [0];

			//Set beam start and end position
			beamComponent->SetActorParameter ("Source", beam);
			beamComponent->SetActorParameter ("Target", _goalTarget);

			EndGame ();
		}
	}
}

void AMinigameSolarController::SetGameController (ASimulationGameController* gameController)
{
	_gameController = gameController;
}

void AMinigameSolarController::StartGame ()
{
	SetActorTickEnabled (true);

	SetScore (0);

	_endPanelShown = false;

	//Enable start UI
	_uiController->Enable (17, 0);

	InitializeGame ();
}

void AMinigameSolarController::StartGamePlay ()
{
	_gameStarted = true;

	//Disable start UI
	_uiController->Disable (17);

	//Enable solarMiniGame UI
	_uiController->Enable (20, 0);

	UpdateBeams ();
}

void AMinigameSolarController::EndGame ()
{
	SetActorTickEnabled (false);

	SetScore (500);
	endScoreText = "You got " + FString::FromInt (_score) + " out of 3000 points";

	//Disable solarMiniGame UI
	_uiController->Disable (20);

	//Enable lose panel UI
	//_uiController->Enable (19, 2);
	//Enable win panel UI
	_uiController->Enable (19, 2);

	EndScreen ();
	
	_endPanelShown = true;
	_gameStarted = false;
	_gameController->StartClickDelay ();
}

void AMinigameSolarController::SetScore (int score)
{
	_score = score;

	scoreText = FString::FromInt (_score);

	Update ();
}

void AMinigameSolarController::GoBackToSimulation ()
{
	//Disable lose panel UI
	//_uiController->Disable (18);
	//Disable win panel UI
	_uiController->Disable (19);

	_gameController->ExitMiniGame ();

	//Clean up everything
	_beamPosition->Destroy ();
	_goalPosition->Destroy ();
	_goalTarget->Destroy ();
	_startBeam->Destroy ();

	for (int i = 0; i < _grid.Num (); i++)
		_grid [i]->Destroy ();

	_grid.Empty ();

	for (int i = 0; i < _mirrors.Num (); i++)
		_mirrors [i]->Destroy ();

	_mirrors.Empty ();
	_mirrorRotations.Empty ();

	for (int i = 0; i < _beams.Num (); i++)
		_beams [i]->Destroy ();

	_beams.Empty ();
}

void AMinigameSolarController::OnMouseClick ()
{
	if (_endPanelShown)
	{
		if (_gameController->CanContinue ())
			GoBackToSimulation ();
	}
	else if (!_gameStarted)
		StartGamePlay ();
	else if (!_controlledUnit)
	{
		FHitResult hit;
		GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

		if (hit.bBlockingHit)
		{
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Mirror"))
				_preparingLifting = true;
		}
	}
}

void AMinigameSolarController::OnMouseRelease ()
{
	if (_lifting)
		PlaceUnit ();

	if (_preparingLifting)
	{
		if (_liftingTimer < 0.15f)
		{
			_preparingLifting = false;
			_liftingTimer = 0.0f;

			RotateUnit ();
		}
	}
}

//Called to bind functionality to input
void AMinigameSolarController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AMinigameSolarController::OnMouseClick);
	PlayerInputComponent->BindAction ("MouseClick", IE_Released, this, &AMinigameSolarController::OnMouseRelease);
}
