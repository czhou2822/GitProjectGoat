// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerManager.h"
#include "Character/Misc/GameMasterInterface.h"

// Sets default values
AGameMasterInterface::AGameMasterInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGameMasterInterface::BeginPlay()
{
	//GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AGameMasterInterface::GMTick, 0.5, true, 0.f);
	//UE_LOG(LogTemp, Warning, TEXT("GameMasterInterface"));

}

