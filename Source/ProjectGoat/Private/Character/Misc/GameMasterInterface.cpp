// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Misc/GameMasterInterface.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "ProjectGoat/ProjectGoatGameMode.h"

// Sets default values
AGameMasterInterface::AGameMasterInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GMAudioComponent_Wind = CreateDefaultSubobject<UAudioComponent>(TEXT("Wind"));

	GMAudioComponent_TreeRusstling = CreateDefaultSubobject<UAudioComponent>(TEXT("TreeRusstling"));

	GMAudioComponent_Music = CreateDefaultSubobject<UAudioComponent>(TEXT("Music"));

}

void AGameMasterInterface::BeginPlay()
{
	//GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AGameMasterInterface::GMTick, 0.5, true, 0.f);
	//UE_LOG(LogTemp, Warning, TEXT("GameMasterInterface"));

	if (SWAmbientWind)
	{
		GMAudioComponent_Wind->Sound = SWAmbientWind;
		GMAudioComponent_Wind->Play();
	}
	
	if (SMAmbientTreeRusstling)
	{
		GMAudioComponent_TreeRusstling->Sound = SMAmbientTreeRusstling;
		GMAudioComponent_Wind->Play();
	}

	if (SWAmbientMusic)
	{
		GMAudioComponent_Music->Sound = SWAmbientMusic;
		GMAudioComponent_Wind->Play();
	}

}



