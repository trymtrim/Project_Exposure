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

		if (_liftingTimer >= 0.25f)
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

	int mirrorCount = 2;

	if (world)
	{
		//Spawn mirrors
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _middlePosition + FVector (300.0f, 200.0f, 0.0f);
		FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

		for (int i = 0; i < mirrorCount; i++)
		{
			AActor* mirror = world->SpawnActor <AActor> (_mirror, spawnPosition, rotator, spawnParams);
			_mirrors.Add (mirror);

			spawnPosition.Y -= 200.0f;
		}

		//Spawn beam start position
		rotator = FRotator (0.0f, 180.0f, 0.0f);
		spawnPosition = _grid [7]->GetActorLocation () + FVector (400.0f, 0.0f, 0.0f);
		_beamPosition = world->SpawnActor <AActor> (_gridSlot, spawnPosition, rotator, spawnParams);
		//Spawn beam end position
		spawnPosition = _grid [0]->GetActorLocation () - FVector (400.0f, 0.0f, 0.0f);
		_goalPosition = world->SpawnActor <AActor> (_goal, spawnPosition, rotator, spawnParams);
	}
}

void AMinigameSolarController::InitializeGrid ()
{
	UWorld* world = GetWorld ();
	
	int x = 3;
	int y = 3;
	int slotSize = 100;

	if (world)
	{
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _middlePosition - FVector ((slotSize * x) / 2, (slotSize * y) / 2, 0.0f);
		FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				//Spawn the unit
				AActor* gridSlot = world->SpawnActor <AActor> (_gridSlot, spawnPosition, rotator, spawnParams);
				_grid.Add (gridSlot);

				spawnPosition.Y += slotSize;
			}

			spawnPosition.Y -= (slotSize * y);
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
		if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Mirror"))
		{
			hit.GetActor ()->SetActorRotation (hit.GetActor ()->GetActorRotation () + FRotator (0.0f, 90.0f, 0.0f));

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
	count = 0;
}

void AMinigameSolarController::CheckBeamHit (FVector position, FRotator rotation, bool firstBeam)
{
	//Spawn the beam
	FActorSpawnParameters spawnParams;
	FVector spawnPosition = position;
	FRotator rotator = rotation;

	AActor* beam = GetWorld ()->SpawnActor <AActor> (_beam, spawnPosition, rotator, spawnParams);
	_beams.Add (beam);

	if (firstBeam)
		_startBeam = beam;

	FHitResult hit;
	FVector start = beam->GetActorLocation ();
	FVector forwardVector = beam->GetActorForwardVector ();
	FVector end = (forwardVector * 1000.0f) + start;
	FCollisionQueryParams collisionParams;

	DrawDebugLine (GetWorld (), start, end, FColor::Green, false, 1, 0, 5);

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

		count++;

		if (count < 5)
			CheckBeamHit (hitMirror->GetActorLocation (), hitMirror->GetActorRotation (), false);
	}
	else
	{
		if (_goalPosition->ActorLineTraceSingle (hit, start + (forwardVector * 75), end, ECC_WorldDynamic, collisionParams))
			EndGame ();
	}
}

/*int AMinigameSolarController::GetSlot (AActor* mirror)
{
	for (int i = 0; i < _grid.Num (); i++)
	{
		if (FVector::Distance (_grid [i]->GetActorLocation (), mirror->GetActorLocation ()) < 5)
			return i + 1;
	}

	return 0;
}*/

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
	//_uiController->Enable (18, 2);
	//Enable win panel UI
	_uiController->Enable (19, 2);

	EndScreen ();
	
	_endPanelShown = true;
	_gameStarted = false;
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
	_startBeam->Destroy ();

	for (int i = 0; i < _grid.Num (); i++)
		_grid [i]->Destroy ();

	_grid.Empty ();

	for (int i = 0; i < _mirrors.Num (); i++)
	_mirrors [i]->Destroy ();

	_mirrors.Empty ();

	for (int i = 0; i < _beams.Num (); i++)
	_beams [i]->Destroy ();

	_beams.Empty ();
}

void AMinigameSolarController::OnMouseClick ()
{
	if (_endPanelShown)
		GoBackToSimulation ();
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
		if (_liftingTimer < 0.25f)
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
