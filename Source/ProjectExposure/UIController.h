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

	void Enable (UUserWidget* uiReference, int order);
	void Disable (UUserWidget* uiReference);

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
	TSubclassOf <UUserWidget> simulationTest;

	//Variables to hold the widgets After Creating it.
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* currentTurnRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* simulationRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* resourcesRef;
	UUserWidget* menuRef;
	UPROPERTY (BlueprintReadOnly, Category = "UI") UUserWidget* drillMiniGameRef;
	UUserWidget* simulationTestRef;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void InitializeWidgets ();
};
