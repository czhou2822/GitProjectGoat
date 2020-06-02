// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Core/BulkheadCharacterBase.h"

// Sets default values
ABulkheadCharacterBase::ABulkheadCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GUID = FGuid::NewGuid();
}

// Called when the game starts or when spawned
void ABulkheadCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GetController())
	{
		SpawnDefaultController();
	}
	
}

// Called every frame
void ABulkheadCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABulkheadCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FCharacterData& ABulkheadCharacterBase::GetCharacterData()
{
	if (GetGameState())
	{
		return GetGameState()->GetCharacterData(GUID);
	}
	return CharacterDataNULL;
}