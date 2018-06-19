//Fill out your copyright notice in the Description page of Project Settings.

#include "UIController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "UserWidget.h"
#include "Slate.h"

//Sets default values
AUIController::AUIController ()
{
 	//Set this actor to call Tick () every frame
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
	videoRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), video); //14
	videoPauseRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), videoPause); //15
	blackStartRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), blackStart); //16
	solarAnimationRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), solarAnimation); //17
	solarLosePanelRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), solarLosePanel); //18
	solarWinPanelRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), solarWinPanel); //19
	solarMiniGameRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), solarMiniGame); //20
	loseRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), lose); //21
	winRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), win); //22
	
	userInterfaces.Add (currentTurnRef);
	userInterfaces.Add (simulationRef);
	userInterfaces.Add (resourcesRef);
	userInterfaces.Add (menuRef);
	userInterfaces.Add (drillMiniGameRef);
	userInterfaces.Add (simulationTestRef);
	userInterfaces.Add (cartMiniGameRef);
	userInterfaces.Add (drillLosePanelRef);
	userInterfaces.Add (drillWinPanelRef);
	userInterfaces.Add (drillAnimationRef);
	userInterfaces.Add (cartLosePanelRef);
	userInterfaces.Add (cartWinPanelRef);
	userInterfaces.Add (cartAnimationRef);
	userInterfaces.Add (drillAnimation2Ref);
	userInterfaces.Add (videoRef);
	userInterfaces.Add (videoPauseRef);
	userInterfaces.Add (blackStartRef);
	userInterfaces.Add (solarAnimationRef);
	userInterfaces.Add (solarLosePanelRef);
	userInterfaces.Add (solarWinPanelRef);
	userInterfaces.Add (solarMiniGameRef);
	userInterfaces.Add (loseRef);
	userInterfaces.Add (winRef);

	if (GetWorld ()->GetMapName ().Mid (GetWorld ()->StreamingLevelsPrefix.Len ()) == "MainLevel_3")
		Enable (16, 2);
}

void AUIController::Enable (int index, int order)
{
	userInterfaces [index]->AddToViewport (order);
}

void AUIController::Disable (int index)
{
	userInterfaces [index]->RemoveFromParent ();
}

//Called every frame
void AUIController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);
}
