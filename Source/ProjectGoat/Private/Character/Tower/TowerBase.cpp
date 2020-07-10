// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Tower/TowerBase.h"

// Sets default values
ATowerBase::ATowerBase()
	:ABulkheadCharacterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterData().bTeam = true;

}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

