// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
	:bIsFiring(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMeshXXBase");
	RootComponent = WeaponBaseMesh;

	FirePoint = CreateDefaultSubobject<USceneComponent>("FirePoint");
	FirePoint->SetupAttachment(RootComponent);

	//WeaponCollisionRange = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponCollision"));

	//WeaponCollisionRange->bHiddenInGame = false;
	//WeaponCollisionRange->bVisible = false;
	GMAudioComponent_Fire = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire"));
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
	bIsFiring = true;
	if (SWFireDown)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SWFireDown);
	}
	if (SWFire) 
	{
		GMAudioComponent_Fire->Sound = SWFire;
		GMAudioComponent_Fire->Play(2.f);
	}
}

void AWeaponBase::FireEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	bShowDebugCollision = false;
	bIsFiring = false;
	if (SWFire)
	{
		//GMAudioComponent_Fire->Sound = SWFire;
		GMAudioComponent_Fire->Stop();
	}
	if (SWFireUp)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SWFireUp);
	}
}

void AWeaponBase::Fire()
{
	//UE_LOG(LogTemp, Log, TEXT("Base Weapon Fire"));
}

