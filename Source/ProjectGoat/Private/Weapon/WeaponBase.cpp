// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Components/ShapeComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMeshXXBase");
	RootComponent = WeaponBaseMesh;

	//WeaponCollisionRange = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponCollision"));

	//WeaponCollisionRange->bHiddenInGame = false;
	//WeaponCollisionRange->bVisible = false;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::FireStart()
{
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeaponBase::Fire, FireInvetval, true, 0.f);
	bShowDebugCollision = true;
	
}

void AWeaponBase::FireEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	bShowDebugCollision = false;
	
}

void AWeaponBase::Fire()
{
	//UE_LOG(LogTemp, Log, TEXT("Base Weapon Fire"));
}

