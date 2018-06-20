//Fill out your copyright notice in the Description page of Project Settings.

#include "Simulation.h"
#include "TimerManager.h"
#include "SimulationGameController.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
ASimulation::ASimulation ()
{
 	//Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void ASimulation::BeginPlay ()
{
	Super::BeginPlay ();
	InitializeResources ();
	ToggleParticlesEvent();

	_currentTurn = 0;
	_currentCityStage = 0;

	_lightLerping = false;
	_lightReversed = false;
	_lightLerpValue = 0;

	//Get Light Components
	if (dayLightActor->IsValidLowLevel()) {
		UDirectionalLightComponent* temp = Cast<UDirectionalLightComponent>(dayLightActor->GetComponentByClass(UDirectionalLightComponent::StaticClass()));
		if (temp->IsValidLowLevel()) {
			dayLight = temp;
		}
	}
	if (nightLightActor->IsValidLowLevel()) {
		UDirectionalLightComponent* temp = Cast<UDirectionalLightComponent>(nightLightActor->GetComponentByClass(UDirectionalLightComponent::StaticClass()));
		if (temp->IsValidLowLevel()) {
			nightLight = temp;
		}
	}

	_insufficientLastRound = false;
	_isSimulation = false;
}

//Called every frame
void ASimulation::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	if (_lightLerping) LerpLights(DeltaTime);
}

void ASimulation::OnPlaceUnit (APlaceableUnit* unit)
{
	switch (unit->GetTypeIndex ())
	{
	case 1: //Nuclear reactor
		AddResources (3.0f * unit->GetPerformancePercentage (), 1.0f);
		_nuclear.Add(unit);
		break;
	case 2: //Windmill
		AddResources (1.0f * unit->GetPerformancePercentage (), 0.0f);
		_solar.Add(unit);
		break;
	case 3: //Oil rig
		AddResources (3.0f * unit->GetPerformancePercentage (), 3.0f);
		_oil.Add(unit);
		break;
	}
}

void ASimulation::OnRemoveUnit(APlaceableUnit* unit) {
	switch(unit->GetTypeIndex ()) 
	{
	case 1: //Nuclear reactor
		AddResources(-3 * unit->GetPerformancePercentage (), 0);
		_nuclear.Remove (unit);
		break;
	case 2: //Windmill
		AddResources(-1 * unit->GetPerformancePercentage (), 0);
		_solar.Remove (unit);
		break;
	case 3: //Oil rig
		AddResources(-3 * unit->GetPerformancePercentage (), 0);
		_oil.Remove (unit);
		break;
	}
}

void ASimulation::OnNewTurn (int currentTurn) {
	_currentTurn = currentTurn;
}

void ASimulation::InitializeResources ()
{
//Maximum resources at start
maxEnergy = 0;
maxPollution = 6;

//Starting resources
currentEnergy = 0;
currentPollution = 0;

_nuclearPollution = 0;
_maxNuclearPollution = 5;

//Misc
_cityEnergyNeed = 2;
}

void ASimulation::AddResources(float energyValue, float pollutionValue) {
	currentEnergy += energyValue;
	currentPollution += pollutionValue;
}

void ASimulation::StartSimulation() {
	_isSimulation = true;
	_lightLerping = true;

	//Increase all the resources 'n stuff
	HandleResources();

	//Calc what the city is gonna do as feedback for this turn
	CalculateFeedback();
}

void ASimulation::StopSimulation() {
	_isSimulation = false;
	_lightLerping = true;

	//Check if the Player screwed up
	CheckForDeath();
}

void ASimulation::LerpLights(float DeltaTime) {

	if (!_lightReversed) _lightLerpValue += DeltaTime;
	else _lightLerpValue -= DeltaTime;

	if (_lightLerpValue > 1.0f && !_lightReversed) {
		_lightLerping = false;
		_lightReversed = true;
		_lightLerpValue = 1.0f;
	}
	if (_lightReversed && _lightLerpValue < 0.0f) {
		_lightLerping = false;
		_lightReversed = false;
		_lightLerpValue = 0.0f;
	}
	
	bool temp = !_lightReversed;
	//!_lightReversed because it is false when it is day -> so we need to swap them around to have true on day and false on night
	UpdateCycle(temp, _lightLerpValue, dayLight, nightLight);
}

void ASimulation::HandleResources() {
	//Every nth turn - right now every 3rd - make the city bigger, increase maximum resources and sizably increase city energy need
	if (_currentTurn % 3 == 0 && _currentTurn > 2) {
		_currentCityStage++;
		if (_currentCityStage < 3) _city->ToggleStage(_currentCityStage);
		_cityEnergyNeed += 3;
	} else {
		_cityEnergyNeed += 1;
	}

	if (_currentTurn % 2 == 1 && _currentTurn != 1) {
		currentPollution -= 2;
	}

	//Loop through all the nuclear powerplants and increase and check the number of turns they exist
	//If they exist for 2 turn, start adding nuclear waste
	for (int i = 0; i < _nuclear.Num(); i++) {
		_nuclear[i]->AddTurn ();
		if (_nuclear[i]->GetTurn () > 2) {
			_nuclearPollution++;
		}
	}
}

void ASimulation::CheckForDeath() {
	bool dead = false;
	//Check if we have enough energy
	if (currentEnergy < _cityEnergyNeed) {
		dead = true;
	}
	//Check if there is too much nuclear waste
	else if (_nuclearPollution > _maxNuclearPollution) {
		dead = true;
	}
	//Check if there is too much pollution
	else if (currentPollution > maxPollution) {
		dead = true;
	}

	if (dead) {
		if (_insufficientLastRound) {
			_controller->EndGame(false);
		} else {
			_insufficientLastRound = true;
		}
	} else {
		_insufficientLastRound = false;
	}
	
	if (_currentTurn == 9 && !dead) _controller->EndGame(true);
}

void ASimulation::CalculateFeedback() {
	//How much percent of the city energy need is our currentEnergy 
	float percentage = currentEnergy / _cityEnergyNeed * 100;

	//If we are negative
	if (percentage < 100) {
		//0 -> X
		if (percentage < _feedbackNegativeScales.X) {

		} 
		//X -> Y
		else if (percentage >= _feedbackNegativeScales.X && percentage < _feedbackNegativeScales.Y) {

		} 
		//Y -> Z
		else if (percentage >= _feedbackNegativeScales.Y && percentage < _feedbackNegativeScales.Z) {

		}
		//Z -> 99
		else {

		}
	} 
	//If we are positive
	else if (percentage > 100) {
		//101 -> X
		if (percentage < _feedbackPositiveScales.X) {

		}
		//X -> Y
		else if (percentage >= _feedbackPositiveScales.X && percentage < _feedbackPositiveScales.Y) {

		}
		//Y -> Z
		else if (percentage >= _feedbackPositiveScales.Y && percentage < _feedbackPositiveScales.Z) {

		}
		//Z -> infinity baby x)
		else {

		}
	}
	//If we are excatly at 100% needed energy
	else {
	
	}
}

void ASimulation::ToggleParticles() {
	particlesActive = !particlesActive;
	ToggleParticlesEvent();
}
