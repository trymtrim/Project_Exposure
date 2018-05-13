//Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
//#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
//#include "MyProjectCharacter.h"
#include "Engine/World.h"
#include "EngineGlobals.h"
#include "Engine.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

AMainPlayerController::AMainPlayerController ()
{
	bShowMouseCursor = true;

	/*bAutoManageActiveCameraTarget = false;
	PlayerCameraManagerClass = AMainCameraManager::StaticClass ();
	SetViewTarget (GetPawn ());*/

	//DefaultMouseCursor = EMouseCursor::Crosshairs;
	//bAutoManageActiveCameraTarget = true;
}

void AMainPlayerController::PlayerTick (float deltaTime)
{
	Super::PlayerTick (deltaTime);

	// keep updating the destination every tick while desired
	/*if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}*/
}

void AMainPlayerController::PlaceUnit ()
{
	print ("YOO");
}

void AMainPlayerController::SetupInputComponent ()
{
	//Set up gameplay key bindings
	Super::SetupInputComponent ();

	//InputComponent->BindAction ("MouseClick", IE_Pressed, this, &AMainPlayerController::PlaceUnit);

	/*InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMyProjectPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMyProjectPlayerController::OnSetDestinationReleased);*/
}
