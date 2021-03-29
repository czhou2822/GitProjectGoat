// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Misc/EnemyRouteSegment.h"


#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AEnemyRouteSegment::AEnemyRouteSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavSplinePoints = CreateDefaultSubobject<USplineComponent>(TEXT("NavSplinePoints"));

	NavSplinePoints->SetupAttachment(RootComponent);
	NavSplinePoints->bHiddenInGame = false;
	NavSplinePoints->SetVisibility(true);


}

// Called when the game starts or when spawned
void AEnemyRouteSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyRouteSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

