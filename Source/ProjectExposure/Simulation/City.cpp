// Fill out your copyright notice in the Description page of Project Settings.

#include "City.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
ACity::ACity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACity::BeginPlay()
{
	Super::BeginPlay();
	_stageActives.Init(false, 3);
	_stageLerps.Init(0.0f, 3);
	_stageActives[0] = true;
}

// Called every frame
void ACity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LerpStages(DeltaTime);
}

void ACity::ToggleStage(int pStage) {
	if(pStage < _stageActives.Num()) _stageActives[pStage] = !_stageActives[pStage];
	else print(FString::FromInt(pStage));
}

void ACity::LerpStages(float DeltaTime) {
	//Go through all of our city stages
	for (int32 i = 0; i < _stageActives.Num(); i++) {
		//Check if the stage is active aka. should build up
		if (_stageActives[i]) {
			//If the lerp value is under the value we need, increase it
			if (_stageLerps[i] < 3.0f) {
				_stageLerps[i] += DeltaTime;
			} else {
				_stageLerps[i] = 3.0f;
			}

			//This exists because Unreal does not support nested arrays :(
			TArray< AActor* > currentStage;

			//Get the correct array for our stage
			switch (i) {
			case 0:
				currentStage = _stageOne;
				break;
			case 1:
				currentStage = _stageTwo;
				break;
			case 2:
				currentStage = _stageThree;
				break;
			default:
				currentStage = _stageOne;
				break;
			}

			//Loop through all the buildings/actor objects withing the stage array
			for (auto& building : currentStage) {
				//Get all the StaticMeshComponents, should be just one
				TArray<UStaticMeshComponent*> meshComponents;
				building->GetComponents<UStaticMeshComponent>(meshComponents);

				//Loop through all the components
				for (int32 j = 0; j < meshComponents.Num(); j++) {

					//Get and loop through all the materials, again should be just one
					TArray<class UMaterialInterface*> materials = meshComponents[j]->GetMaterials();
					for (int k = 0; k < materials.Num(); k++) {
						//Create a dynamic material
						UMaterialInstanceDynamic* matInstance = UMaterialInstanceDynamic::Create(_parentMaterial, building);

						//Modify the lerp value for the buildup 
						if (matInstance != nullptr) {
							matInstance->SetScalarParameterValue("LerpValue", _stageLerps[i]);
						}
						//Apply changed material to the mesh component
						meshComponents[j]->SetMaterial(k, matInstance);
					}
				}
			}
		}
	}
}

