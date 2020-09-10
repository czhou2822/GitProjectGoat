// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Misc/EnemyRoute.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AEnemyRoute::AEnemyRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;


	NavSplinePoints = CreateDefaultSubobject<USplineComponent>(TEXT("NavSplinePoints"));

	NavSplinePoints->SetupAttachment(RootComponent);
	NavSplinePoints->bHiddenInGame = false;
	NavSplinePoints->SetVisibility(true);

}

void AEnemyRoute::RouteSplineToArray()
{
	NavPoints.Empty();
	for (int32 i = 0; i < NavSplinePoints->GetNumberOfSplinePoints(); i++)
	{
		NavPoints.Add(NavSplinePoints->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}
}

void AEnemyRoute::SetSplineMeshesIsHidden(const bool bIsHidden)
{
	for (auto& Tmp : SplineMeshes)
	{
		if (Tmp)
		{
			Tmp->SetHiddenInGame(bIsHidden);
		}
	}
	return;
}

// Called when the game starts or when spawned
void AEnemyRoute::BeginPlay()
{
	Super::BeginPlay();
	//CreateSplineMesh();
	RouteSplineToArray();
	SetSplineMeshesIsHidden(true);
}


void AEnemyRoute::CreateSplineMesh()
{

	for (int32 i = 0; i <= NavSplinePoints->GetNumberOfSplinePoints() - 2; i++)
	{
		FVector ThisSplineLocation, ThisSplineTangent;
		FVector NextSplineLocation, NextSplineTangent;

		NavSplinePoints->GetLocationAndTangentAtSplinePoint(i, ThisSplineLocation, ThisSplineTangent,ESplineCoordinateSpace::World);
		NavSplinePoints->GetLocationAndTangentAtSplinePoint(i+1, NextSplineLocation, NextSplineTangent, ESplineCoordinateSpace::World);

		//FName CompName = 

		USplineMeshComponent* NewSplineMeshComp = NewObject<USplineMeshComponent>(this);
		SplineMeshes.Add(NewSplineMeshComp);
		NewSplineMeshComp->SetStartAndEnd(ThisSplineLocation, ThisSplineTangent, NextSplineLocation, NextSplineTangent, true);

	}
}




