// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Misc/EnemyRoute.h"
#include "Character/Misc/EnemyRouteSegment.h"



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
	//navpoint were created backwards in editor -> reads in backward
	NavPoints.Empty();

	//for (int32 i = NavSplinePoints->GetNumberOfSplinePoints() - 1; i >= 0; i--)
	//{
	//	NavPoints.Add(NavSplinePoints->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	//}

	for (auto TmpSegment : RouteSegments)
	{
		for (int32 i = 0; i < TmpSegment->NavSplinePoints->GetNumberOfSplinePoints(); i++)
		{
			FVector NextPoint = TmpSegment->NavSplinePoints->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			if (NavPoints.Num())
			{
				//check range to add
				FVector LastVector = NavPoints[NavPoints.Num()-1];
				float Distance = FVector::Dist(LastVector, NextPoint);
				if (Distance <= 100.f)
				{
					continue;
				}
			}
			NavPoints.Add(NextPoint);
		}
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




