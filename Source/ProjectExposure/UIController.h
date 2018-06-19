//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIController.generated.h"

class UUserWidget;

UCLASS()
class PROJECTEXPOSURE_API AUIController : public AActor
{
	GENERATED_BODY ()
	
public:	
	//Sets default values for this actor's properties
	AUIController ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	/*
	currentTurnRef = 0			simulationRef = 1
	resourcesRef = 2			menuRef = 3
	drillMiniGameRef = 4		simulationTestRef = 5
	cartMiniGameRef = 6			drillLosePanelRef = 7
	drillWinPanelRef = 8		drillAnimationRef = 9
	cartLosePanelRef = 10		cartWinPanelRef = 11
	cartAnimationRef = 12		drillAnimation2Ref = 13
	videoRef = 14				videoPauseRef = 15
	blackStartRef = 16			solarAnimationRef = 17
	thrashRef = 18		solarWinPanelRef = 19
	solarMiniGameRef = 20		loseRef = 21
	winRef = 22
	*/
	void Enable (int index, int order);
	/*
	currentTurnRef = 0			simulationRef = 1
	resourcesRef = 2			menuRef = 3
	drillMiniGameRef = 4		simulationTestRef = 5
	cartMiniGameRef = 6			drillLosePanelRef = 7
	drillWinPanelRef = 8		drillAnimationRef = 9
	cartLosePanelRef = 10		cartWinPanelRef = 11
	cartAnimationRef = 12		drillAnimation2Ref = 13
	videoRef = 14				videoPauseRef = 15
	blackStartRef = 16			solarAnimationRef = 17
	thrashRef = 18		solarWinPanelRef = 19
	solarMiniGameRef = 20		loseRef = 21
	winRef = 22
	*/
	void Disable (int index);

	//Reference UMG Assets in the Editor
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> currentTurn;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> simulation;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> resources;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> menu;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> drillMiniGame;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> cartMiniGame;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> drillLosePanel;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> drillWinPanel;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> drillAnimation;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> cartLosePanel;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> cartWinPanel;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> cartAnimation;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> drillAnimation2;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> video;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> videoPause;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> blackStart;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> simulationTest;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> solarAnimation;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> solarLosePanel;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> solarWinPanel;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> solarMiniGame;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> lose;
	UPROPERTY (EditAnywhere)
	TSubclassOf <UUserWidget> win;

	//Variables to hold the widgets After Creating it.
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* currentTurnRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* simulationRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* resourcesRef;
	UPROPERTY () UUserWidget* menuRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* drillMiniGameRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* cartMiniGameRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* drillLosePanelRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* drillWinPanelRef;
	UPROPERTY () UUserWidget* drillAnimationRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* cartLosePanelRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* cartWinPanelRef;
	UPROPERTY () UUserWidget* cartAnimationRef;
	UPROPERTY () UUserWidget* drillAnimation2Ref;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* videoRef;
	UPROPERTY () UUserWidget* videoPauseRef;
	UPROPERTY () UUserWidget* blackStartRef;
	UPROPERTY () UUserWidget* simulationTestRef;
	UPROPERTY () UUserWidget* solarAnimationRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* solarLosePanelRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* solarWinPanelRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* solarMiniGameRef;
	UPROPERTY () UUserWidget* loseRef;
	UPROPERTY () UUserWidget* winRef;

	TArray <UUserWidget*> userInterfaces;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void InitializeWidgets ();
};
