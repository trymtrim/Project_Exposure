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
	TSubclassOf <UUserWidget> simulationTest;

	//Variables to hold the widgets After Creating it.
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* currentTurnRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* simulationRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* resourcesRef;
	UUserWidget* menuRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* drillMiniGameRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* cartMiniGameRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* drillLosePanelRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* drillWinPanelRef;
	UUserWidget* drillAnimationRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* cartLosePanelRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* cartWinPanelRef;
	UUserWidget* cartAnimationRef;
	UUserWidget* drillAnimation2Ref;
	UUserWidget* simulationTestRef;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void InitializeWidgets ();
};
