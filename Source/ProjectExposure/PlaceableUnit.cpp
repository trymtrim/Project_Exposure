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

void APlaceableUnit::BeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	print ("YO");
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
		//if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Placeable"))
		//{
			float objectHeight = _mesh->CalcBounds (GetTransform ()).BoxExtent.Z;
			
			FVector position = FVector (hit.Location.X, hit.Location.Y, hit.Location.Z + objectHeight);
			SetActorLocation (position);//

			//Change outline color
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Placeable"))
				_mesh->SetCustomDepthStencilValue (0);
			else
				_mesh->SetCustomDepthStencilValue (1);

			_mesh->MarkRenderStateDirty ();
		//}
	}

	/*TArray <UStaticMeshComponent*> StaticComps;
	GetComponents <UStaticMeshComponent> (StaticComps);
	
	for (int i = 0; i < StaticComps.Num (); i++)
		print (StaticComps [i]->GetName ());*/
}

bool APlaceableUnit::PlaceUnit ()
{
	//if (_canPlace)
	//	return false;

	_isPlaced = true;

	//Disable outline
	_mesh->bRenderCustomDepth = false;
	_mesh->MarkRenderStateDirty ();

	return true;
}

void APlaceableUnit::SetMaterials (UMaterialInterface* normalMaterial, UMaterialInterface* collideMaterial)
{
	_normalMaterial = normalMaterial;
	_collideMaterial = collideMaterial;
}
