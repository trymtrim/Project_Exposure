//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlaceableUnit.h"
#include "SimulationGameController.generated.h"

UCLASS()
class PROJECTEXPOSURE_API ASimulationGameController : public APawn
{
	GENERATED_BODY ()

public:
	ASimulationGameController ();

public:	
	// Called every frame
	virtual void Tick (float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	UFUNCTION (BlueprintCallable, Category = "ButtonPress")
	void SpawnUnit (int index);

	//void OnMouseClickEvent ();

protected:
	virtual void BeginPlay () override;

private:
	void PlaceUnit ();

	//UPROPERTY ()
	APlaceableUnit* _controlledUnit = nullptr;

	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _nuclearReactorPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _windmillPrefab;
	UPROPERTY (EditAnywhere)
	TSubclassOf <AActor> _oilRigPrefab;
};
