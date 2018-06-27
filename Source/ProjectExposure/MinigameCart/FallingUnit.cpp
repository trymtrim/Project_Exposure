// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingUnit.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
AFallingUnit::AFallingUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFallingUnit::BeginPlay()
{
	Super::BeginPlay();
}

void AFallingUnit::init(UnitType pType, float pSpeed, float pDeathThreshold) {
	_currentType = pType;
	_speed = pSpeed;
	_deathThreshhold = pDeathThreshold;

	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);

	//Set type and mesh accordingly
	if (_currentType == UnitType::DEBRIS) {
		for (int32 i = 0; i<Components.Num(); i++) {
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			StaticMeshComponent->SetStaticMesh(debrisMesh);
		}
	} else {
		for (int32 i = 0; i<Components.Num(); i++) {
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			StaticMeshComponent->SetStaticMesh(uraniumMesh);
		}
	}
}

UnitType AFallingUnit::getType() {
	return _currentType;
}

//For more info look into FallinUnit Blueprint
void AFallingUnit::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	if(_currentType == UnitType::URANIUM) _isDissolving = true;
	else Destroy();
}

// Called every frame
void AFallingUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_isDissolving) {
		_dissolveValue -= DeltaTime * 7.5f;

		//Get all the StaticMeshComponents, should be just one
		TArray<UStaticMeshComponent*> meshComponents;
		GetComponents<UStaticMeshComponent>(meshComponents);

		//Loop through all the components
		for (int32 j = 0; j < meshComponents.Num(); j++) {

			//Get and loop through all the materials, again should be just one
			TArray<class UMaterialInterface*> materials = meshComponents[j]->GetMaterials();
			for (int k = 0; k < materials.Num(); k++) {
				//Create a dynamic material
				UMaterialInstanceDynamic* matInstance = UMaterialInstanceDynamic::Create(_parentMaterial, this);

				//Modify the lerp value for the buildup 
				if (matInstance != nullptr) {
					matInstance->SetScalarParameterValue("LerpValue", _dissolveValue);
				}
				//Apply changed material to the mesh component
				meshComponents[j]->SetMaterial(k, matInstance);
			}
		}

		if (_dissolveValue < 0.0f) Destroy();
	}
	float tempSpeed = _speed;
	if (_isDissolving) tempSpeed /= 1.75f;
	FVector NewLocation = GetActorLocation() + (FVector(0.0f, 0.0f, -tempSpeed * 100.0f) * DeltaTime);
	SetActorLocation(NewLocation);
	if (NewLocation.Z < _deathThreshhold) Destroy();
}

