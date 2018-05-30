//Fill out your copyright notice in the Description page of Project Settings.

#include "UIController.h"
#include "UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Slate.h"

//Sets default values
AUIController::AUIController ()
{
 	//Set this actor to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void AUIController::BeginPlay ()
{
	Super::BeginPlay ();

	InitializeWidgets ();
}

void AUIController::InitializeWidgets ()
{
	//Create the widgets and store them
	currentTurnRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), currentTurn);
	simulationRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), simulation);
	resourcesRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), resources);
	menuRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), menu);
	drillMiniGameRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), drillMiniGame);
	simulationTestRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), simulationTest);
}

void AUIController::Enable (UUserWidget* uiReference, int order)
{
	//Check if the Asset is assigned in the blueprint, then add it to the view port
	uiReference->AddToViewport (order);
}

void AUIController::Disable (UUserWidget* uiReference)
{
	uiReference->RemoveFromParent ();
}

//Called every frame
void AUIController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);
}
