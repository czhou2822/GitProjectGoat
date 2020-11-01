// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Misc/Trap.h"

#include "Components/StaticMeshComponent.h" 
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"




// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
	RootComponent = StaticMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TrapCollision"));
	BoxComponent->SetupAttachment(RootComponent);

	FacingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FacingArrow"));
	FacingArrow->SetupAttachment(RootComponent);

	NaviPoint = CreateDefaultSubobject<USceneComponent>(TEXT("NavPoint"));
	NaviPoint->SetupAttachment(FacingArrow);

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ATrap::GetBaseNavPoint()
{
	if (NaviPoint)
	{
		return NaviPoint->GetComponentLocation();
	}
	return FVector::ZeroVector;
}

