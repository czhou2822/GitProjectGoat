// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Misc/EnemyRoute.h"
#include "Components/SplineComponent.h"

// Sets default values
AEnemyRoute::AEnemyRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NavSplinePoints = CreateDefaultSubobject<USplineComponent>(TEXT("NavSplinePoints"));

	NavSplinePoints->SetupAttachment(RootComponent);
	NavSplinePoints->bHiddenInGame = false;
	NavSplinePoints->bVisible = true;

}

// Called when the game starts or when spawned
void AEnemyRoute::BeginPlay()
{
	Super::BeginPlay();
	
}



