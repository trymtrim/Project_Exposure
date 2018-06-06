//Fill out your copyright notice in the Description page of Project Settings.

#include "UIController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "UserWidget.h"
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
	currentTurnRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), currentTurn); //0
	simulationRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), simulation); //1
	resourcesRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), resources); //2
	menuRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), menu); //3
	drillMiniGameRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), drillMiniGame); //4
	simulationTestRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), simulationTest); //5
	cartMiniGameRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), cartMiniGame); //6
	drillLosePanelRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), drillLosePanel); //7
	drillWinPanelRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), drillWinPanel); //8
	drillAnimationRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), drillAnimation); //9
	cartLosePanelRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), cartLosePanel); //10
	cartWinPanelRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), cartWinPanel); //11
	cartAnimationRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), cartAnimation); //12
	drillAnimation2Ref = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), drillAnimation2); //13
}

void AUIController::Enable (int index, int order)
{
	switch (index)
	{
	case 0:
		currentTurnRef->AddToViewport (order);
		break;
	case 1:
		simulationRef->AddToViewport (order);
		break;
	case 2:
		resourcesRef->AddToViewport (order);
		break;
	case 3:
		menuRef->AddToViewport (order);
		break;
	case 4:
		drillMiniGameRef->AddToViewport (order);
		break;
	case 5:
		simulationTestRef->AddToViewport (order);
		break;
	case 6:
		cartMiniGameRef->AddToViewport (order);
		break;
	case 7:
		drillLosePanelRef->AddToViewport (order);
		break;
	case 8:
		drillWinPanelRef->AddToViewport (order);
		break;
	case 9:
		drillAnimationRef->AddToViewport (order);
		break;
	case 10:
		cartLosePanelRef->AddToViewport (order);
		break;
	case 11:
		cartWinPanelRef->AddToViewport (order);
		break;
	case 12:
		cartAnimationRef->AddToViewport (order);
		break;
	case 13:
		drillAnimation2Ref->AddToViewport (order);
		break;
	}

	//Check if the Asset is assigned in the blueprint, then add it to the view port
	//uiReference->AddToViewport (order);
}

void AUIController::Disable (int index)
{
	switch (index)
	{
	case 0:
		currentTurnRef->RemoveFromParent ();
		break;
	case 1:
		simulationRef->RemoveFromParent ();
		break;
	case 2:
		resourcesRef->RemoveFromParent ();
		break;
	case 3:
		menuRef->RemoveFromParent ();
		break;
	case 4:
		drillMiniGameRef->RemoveFromParent ();
		break;
	case 5:
		simulationTestRef->RemoveFromParent ();
		break;
	case 6:
		cartMiniGameRef->RemoveFromParent ();
		break;
	case 7:
		drillLosePanelRef->RemoveFromParent ();
		break;
	case 8:
		drillWinPanelRef->RemoveFromParent ();
		break;
	case 9:
		drillAnimationRef->RemoveFromParent ();
		break;
	case 10:
		cartLosePanelRef->RemoveFromParent ();
		break;
	case 11:
		cartWinPanelRef->RemoveFromParent ();
		break;
	case 12:
		cartAnimationRef->RemoveFromParent ();
		break;
	case 13:
		drillAnimation2Ref->RemoveFromParent ();
		break;
	}
}

//Called every frame
void AUIController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);
}
