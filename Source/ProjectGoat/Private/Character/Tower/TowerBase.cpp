// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Tower/TowerBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Qi/TPSCharacterQ.h"


// Sets default values
ATowerBase::ATowerBase()
	:ABulkheadCharacterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterData().bTeam = true;

	ATPSCharacterQ* TPSCharacter = Cast<ATPSCharacterQ>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (TPSCharacter)
	{
		TPSCharacter->OnTowerPlaced.AddDynamic(this, &ATowerBase::HandleOnTowerPlaced);
	}
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

void ATowerBase::HandleOnTowerPlaced()
{
	UE_LOG(LogTemp, Warning, TEXT("TowerPlaced"));
}

