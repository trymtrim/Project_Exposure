//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DirectionalLightComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "City.h"
#include "PlaceableUnit.h"
#include "Simulation.generated.h"

class ASimulationGameController;
class AEmitter;

UCLASS(Blueprintable)
class PROJECTEXPOSURE_API ASimulation : public AActor
{
	GENERATED_BODY ()
	
public:	
	//Sets default values for this actor's properties
	ASimulation ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called from SimulationGameController when a unit is placed
	void OnPlaceUnit (APlaceableUnit* unit);
	void OnRemoveUnit(APlaceableUnit* unit);

	void OnNewTurn (int currentTurn);
	
	UPROPERTY(EditAnywhere)
	ASimulationGameController* _controller;

	UPROPERTY(EditAnywhere)
	ACity* _city;

	UPROPERTY(EditAnywhere)
	AActor* dayLightActor;
	
	UPROPERTY()
	UDirectionalLightComponent* dayLight;

	UPROPERTY(EditAnywhere)
	AActor* nightLightActor;

	UPROPERTY()
	UDirectionalLightComponent* nightLight;
	
	UPROPERTY(EditAnywhere)
	TArray<AEmitter*> positiveParticles;
	
	UPROPERTY(EditAnywhere)
	TArray<AEmitter*> negativeParticles;

	UPROPERTY(EditAnywhere)
	TSubclassOf <AActor> nuclearWastePrefab;

	//Values for the energy and pollution bars //To make blocks, put a transparent sprite in front of the bars or something
	UPROPERTY (BlueprintReadOnly, VisibleAnywhere)
	float maxEnergy;
	UPROPERTY (BlueprintReadOnly, VisibleAnywhere)
	float currentEnergy;
	UPROPERTY (BlueprintReadOnly, VisibleAnywhere)
	float maxPollution;
	UPROPERTY (BlueprintReadOnly, VisibleAnywhere)
	float currentPollution;

	UPROPERTY(BlueprintReadOnly)
	bool particlesActive = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Feedback")
	void ToggleParticlesEvent(const TArray<AEmitter*>& particles);

	UFUNCTION(BlueprintImplementableEvent, Category = "Feedback")
	void UpdateCycle(bool isDay, float lerpValue, UDirectionalLightComponent* pDayLight, UDirectionalLightComponent* pNightLight);

	UFUNCTION(BlueprintImplementableEvent, Category = "Feedback")
	void UpdateTower(float towerStage, FVector color);

	void StartSimulation();
	void StopSimulation();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void InitializeResources ();
	/*
	* Use to add/remove resources, positive values for adding, negative values for removing
	*/
	void AddResources(float energyValue, float pollutionValue);


	void ToggleParticles(bool pActive, bool positive);
	void CalculateFeedback();
	void HandleResources();
	void CheckForDeath();
	void LerpLights(float DeltaTime);
	void ScaleNuclearWaste (float deltaTime);
	void ScaleFog (float deltaTime);

	bool _isSimulation;

	bool _lightLerping;
	bool _lightReversed;
	float _lightLerpValue;

	int _currentTurn;
	int _currentCityStage;

	UPROPERTY ()
	AActor* _currentWaste;
	float _wasteLerpValue;

	bool _wasteAddedThisTurn;

	float _fogLerpValue = 0.0f;
	bool _isScalingFog = false;
	FVector _originalFogScale;
	FVector _targetFogScale;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> _fogStageOne;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> _fogStageTwo;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> _fogStageThree;
	
	UPROPERTY(EditAnywhere)
	FVector _feedbackNegativeScales;
	UPROPERTY(EditAnywhere)
	FVector _feedbackPositiveScales;
	
	UPROPERTY(EditAnywhere)
	FVector _colorInsufficient;
	UPROPERTY(EditAnywhere)
	FVector _colorSufficient;
	UPROPERTY(EditAnywhere)
	FVector _colorOversufficient;

	UPROPERTY(VisibleAnywhere)
	int _cityEnergyNeed;
	bool _insufficientLastRound;

	UPROPERTY(VisibleAnywhere)
	int _nuclearPollution;
	UPROPERTY(VisibleAnywhere)
	int _maxNuclearPollution;

	TArray<APlaceableUnit*> _nuclear;
	TArray<APlaceableUnit*> _solar;
	TArray<APlaceableUnit*> _oil;
};
