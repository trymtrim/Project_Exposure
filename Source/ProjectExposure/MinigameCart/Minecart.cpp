// Fill out your copyright notice in the Description page of Project Settings.

#include "Minecart.h"
#include "FallingUnit.h"
#include "MinigameCartController.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
AMinecart::AMinecart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinecart::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMinecart::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	
	//Determine which type of unit we hit and react accordingly
	UnitType type = Cast<AFallingUnit>(OtherActor)->getType();

	if (type == UnitType::URANIUM) {
		_controller->addPoints();
		//print("Collided with Uranium");
	}
	else {
		_controller->decreaseLives();
		//print("Collided with Debris");
	}
}

void AMinecart::setController(AMinigameCartController* pController) {
	_controller = pController;
}

// Called every frame
void AMinecart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

