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
	GMAudioComponent_Fire->OnAudioFinished.AddDynamic(this, &AWeaponBase::HandleOnAudioFinished);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::HandleOnAudioFinished()
{
	if (GMAudioComponent_Fire->Sound == SWFireStart)
	{
		if (SWFire)
		{
			GMAudioComponent_Fire->SetSound(SWFire);
			GMAudioComponent_Fire->Play();
		}
	}
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

	GMAudioComponent_Fire->Stop();

	if (SWFireStart)
	{
		//UGameplayStatics::PlaySound2D(GetWorld(), SWFireStart);
		GMAudioComponent_Fire->SetSound(SWFireStart);
		GMAudioComponent_Fire->Play();
	}

}

void AWeaponBase::FireEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	bShowDebugCollision = false;
	bIsFiring = false;

	GMAudioComponent_Fire->Stop();
	
	if (SWFireEnd)
	{
		GMAudioComponent_Fire->SetSound(SWFireEnd);
		GMAudioComponent_Fire->Play();
	}
}

void AWeaponBase::Fire()
{
	//UE_LOG(LogTemp, Log, TEXT("Base Weapon Fire"));
}

