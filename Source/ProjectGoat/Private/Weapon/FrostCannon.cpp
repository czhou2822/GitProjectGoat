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
	FrostHitCapsule->SetVisibility(true);

	FrostHitCapsule->SetupAttachment(RootComponent);

}



