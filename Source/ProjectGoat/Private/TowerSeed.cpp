// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerSeed.h"
#include "GameFramework/Actor.h"
#include "Qi/TPSCharacterQ.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"


// Sets default values
ATowerSeed::ATowerSeed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Seed = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//seed->InitBoxExtent(FVector(15.0f));
	RootComponent = Seed;

	SeedMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	SeedMovementComponent->SetUpdatedComponent(Seed);
	SeedMovementComponent->InitialSpeed = 1500.f;
	SeedMovementComponent->MaxSpeed = 3000.0f;
	SeedMovementComponent->bRotationFollowsVelocity = true;
	SeedMovementComponent->bShouldBounce = false;
	SeedMovementComponent->Bounciness = 0.3f;
	Seed->BodyInstance.SetCollisionProfileName(TEXT("towerSeed"));
	Seed->OnComponentHit.AddDynamic(this, &ATowerSeed::OnHit);
	Seed->OnComponentBeginOverlap.AddDynamic(this, &ATowerSeed::onOverlap);
	

}


// Called when the game starts or when spawned
void ATowerSeed::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerSeed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATowerSeed::FireInDirection(const FVector& ShootDirection)
{
	SeedMovementComponent->Velocity = ShootDirection * SeedMovementComponent->InitialSpeed;

}

void ATowerSeed::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//Hit.Location;
	UE_LOG(LogTemp, Warning, TEXT("i hit the ground"));

}

void ATowerSeed::onOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("i hit the ground"));
	Seed->DestroyComponent();
}

void ATowerSeed::buildTower()
{

}

