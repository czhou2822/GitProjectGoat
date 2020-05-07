// Fill out your copyright notice in the Description page of Project Settings.



#include "Weapon/FrostCannon.h"
#include "Components/CapsuleComponent.h"
#include "Qi/TPSCharacterQ.h"
#include "Kismet/GameplayStatics.h"


AFrostCannon::AFrostCannon()
{
	//Super::AWeaponBase();
	UE_LOG(LogTemp, Log, TEXT("Frost cannon constructed."));


	FrostHitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FrostHitCapsule"));
	FrostHitCapsule->SetHiddenInGame(false);
	FrostHitCapsule->bVisible = true;

	FrostHitCapsule->SetupAttachment(RootComponent);

}

void AFrostCannon::Fire()
{
	Super::Fire();
	OnFireStart();
}

void AFrostCannon::FireEnd()
{
	Super::FireEnd();
	OnFireEnd();
	UE_LOG(LogTemp, Log, TEXT("Frost Weapon Fire"));
}

