//Fill out your copyright notice in the Description page of Project Settings.

#include "Simulation.h"
#include "TimerManager.h"
#include "SimulationGameController.h"
#include "Particles/Emitter.h"

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

	//Deactivate both particle systems on start
	ToggleParticlesEvent(positiveParticles);
	ToggleParticlesEvent(negativeParticles);

	_currentTurn = 0;
	_currentCityStage = 0;

	_lightLerping = false;
	_lightReversed = false;
	_lightLerpValue = 0;

	_wasteLerpValue = 0;

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

	if (_currentWaste)
		ScaleNuclearWaste (DeltaTime);

	if (_isScalingFog)
		ScaleFog (DeltaTime);
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

void ASimulation::ScaleNuclearWaste (float deltaTime)
{
	_wasteLerpValue += deltaTime * 0.25f;
	_currentWaste->SetActorScale3D (FMath::Lerp (FVector (1.0f, 1.0f, 1.0f), FVector (30.0f, 30.0f, 1.0f), _wasteLerpValue));

	if (_currentWaste->GetActorScale3D ().X >= 30.0f)
	{
		_wasteLerpValue = 0.0f;
		_currentWaste = nullptr;
	}
}

void ASimulation::ScaleFog (float deltaTime)
{
	_fogLerpValue += deltaTime * 0.4f;

	if (_currentCityStage >= 0) {
		for (int i = 0; i < _fogStageOne.Num(); i++) {
			_fogStageOne[i]->SetActorScale3D(FMath::Lerp(_originalFogScale, _targetFogScale, _fogLerpValue));
		}
	}
	if (_currentCityStage > 0) {
		for (int j = 0; j < _fogStageTwo.Num(); j++) {
			_fogStageTwo[j]->SetActorScale3D(FMath::Lerp(_originalFogScale, _targetFogScale, _fogLerpValue));
		}
	}
	if (_currentCityStage > 1) {
		for (int k = 0; k < _fogStageThree.Num(); k++) {
			_fogStageThree[k]->SetActorScale3D(FMath::Lerp(_originalFogScale, _targetFogScale, _fogLerpValue));
		}
	}
	

	if (_fogLerpValue >= 1)
	{
		_fogLerpValue = 0.0f;
		_isScalingFog = false;
	}
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
}

void ASimulation::StopSimulation() {
	_isSimulation = false;
	_lightLerping = true;

	//Deactivate all possible particle systems
	ToggleParticles(false, true);
	ToggleParticles(false, false);

	//Check if the Player screwed up
	CheckForDeath();
}

void ASimulation::LerpLights(float DeltaTime) {

	if (!_lightReversed) _lightLerpValue += DeltaTime * 0.6f;
	else _lightLerpValue -= DeltaTime * 0.6f;

	if (_lightLerpValue > 1.0f && !_lightReversed) {
		_lightLerping = false;
		_lightReversed = true;
		_lightLerpValue = 1.0f;

		//Calc what the city is gonna do as feedback for this turn
		CalculateFeedback ();
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
	_wasteAddedThisTurn = false;
	for (int i = 0; i < _nuclear.Num(); i++) {
		_nuclear[i]->AddTurn ();
		if (_nuclear[i]->GetTurn () > 2) {
			_nuclearPollution++;
			_wasteAddedThisTurn = true;
			if (_nuclear [i]->GetTurn () == 3)
			{
				FActorSpawnParameters spawnParams;
				FVector spawnPosition = _nuclear [i]->GetActorLocation ();
				FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

				_currentWaste = GetWorld ()->SpawnActor <AActor> (nuclearWastePrefab, spawnPosition, rotator, spawnParams);
			}
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

	//If we hit a lose condition this turn, check if we already screwed up the last turn, if yes game over, if no give the player one turn to fix his problems
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
	int happiness = 0;

	//How much percent of the city energy need is our currentEnergy 
	float percentage = currentEnergy / _cityEnergyNeed * 100;

	//If we are negative
	if (percentage < 100) {
		//X -> Y
		//0 - 60
		if (percentage >= _feedbackNegativeScales.X && percentage < _feedbackNegativeScales.Y) {
		} 
		//Y -> Z
		//60 - 80
		else if (percentage >= _feedbackNegativeScales.Y && percentage < _feedbackNegativeScales.Z) {
			UpdateTower(-0.8f, _colorInsufficient);
		}
		//Z -> 99
		//80 - 99
		else {
			UpdateTower(-0.6f, _colorInsufficient);
		}
		happiness -= 1;
		
	} 
	//If we are positive
	else if (percentage >= 100) {
		//X -> Y
		//100 - 120
		if (percentage >= _feedbackPositiveScales.X && percentage < _feedbackPositiveScales.Y) {
			happiness += 1;
			UpdateTower(-0.4f, _colorSufficient);
		}
		//Y -> Z
		//120 - 140
		else if (percentage >= _feedbackPositiveScales.Y && percentage < _feedbackPositiveScales.Z) {
			happiness += 2;
			UpdateTower(-0.2f, _colorOversufficient);
		}
		//Z -> infinity baby x)
		// 140 - infinity
		else {
			happiness += 3;
			UpdateTower(-0.0f, _colorOversufficient);
		}
	}

	//Pollution
	if (currentPollution == 0)
		_targetFogScale = FVector (0.1f, 0.1f, 1.0f);
	else {
		_targetFogScale = FVector (3.0f, 3.0f, 1.0f) * currentPollution;
		happiness -= currentPollution / 2;
	}
	

	if(_wasteAddedThisTurn) happiness -= 1;

	if (happiness < 0) ToggleParticles(true, false);
	else ToggleParticles(true, true);

	_originalFogScale = _fogStageOne[0]->GetActorScale3D ();
	_isScalingFog = true;

}

void ASimulation::ToggleParticles(bool pActive, bool positive) {
	particlesActive = pActive;
	if(positive) ToggleParticlesEvent(positiveParticles);
	else ToggleParticlesEvent(negativeParticles);
}
