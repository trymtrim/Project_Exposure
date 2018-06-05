//Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableUnit.h"
#include "Engine/World.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
APlaceableUnit::APlaceableUnit ()
{
 	//Set this actor to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;

	//Collider->OnComponentBeginOverlap.AddDynamic( this, &AMyActor::BeginOverlap );
}

//Called when the game starts or when spawned
void APlaceableUnit::BeginPlay ()
{
	Super::BeginPlay ();

	_playerController = GetWorld ()->GetFirstPlayerController ();

	TArray <UStaticMeshComponent*> staticComps;
	GetComponents <UStaticMeshComponent> (staticComps);
	_mesh = staticComps [0];

	//Enable outline
	_mesh->bRenderCustomDepth = true;
	_mesh->SetCustomDepthStencilValue (1);
	_mesh->MarkRenderStateDirty ();
}

//Called every frame
void APlaceableUnit::Tick (float deltaTime)
{
	Super::Tick (deltaTime);

	if (!_isPlaced)
		FollowMousePosition ();
}

void APlaceableUnit::FollowMousePosition ()
{
	//Trace to see what is under the mouse cursor
	FHitResult hit;
	_playerController->GetHitResultUnderCursor (ECC_Visibility, true, hit);

	if (hit.bBlockingHit)
	{
		float objectHeight = _mesh->CalcBounds (GetTransform ()).BoxExtent.Z;
			
		FVector position = FVector (hit.Location.X, hit.Location.Y, hit.Location.Z);
		SetActorLocation (position);

		//Change outline color
		if (GetRootComponent ()->ComponentHasTag ("Oil"))
		{
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Placeable"))
				_mesh->SetCustomDepthStencilValue (0);
			else
				_mesh->SetCustomDepthStencilValue (1);
		}
		else
		{
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Placeable"))
				_mesh->SetCustomDepthStencilValue (1);
			else
				_mesh->SetCustomDepthStencilValue (0);
		}
		
		_mesh->MarkRenderStateDirty ();
	}
}

bool APlaceableUnit::PlaceUnit ()
{
	if (_mesh->CustomDepthStencilValue == 1)
		return false;

	_isPlaced = true;

	//Disable outline
	_mesh->bRenderCustomDepth = false;
	_mesh->MarkRenderStateDirty ();

	return true;
}

int APlaceableUnit::GetTypeIndex ()
{
	if (GetRootComponent ()->ComponentHasTag ("Nuclear"))
		return 1;
	else if (GetRootComponent ()->ComponentHasTag ("Windmill"))
		return 2;
	else if (GetRootComponent ()->ComponentHasTag ("Oil"))
		return 3;

	return 0;
}
