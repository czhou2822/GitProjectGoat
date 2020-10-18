// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Misc/EnemySpawn.h"
#include "Engine/World.h"
#include "ProjectGoat/ProjectGoatGameMode.h"
#include "Character/Enemy/EnemyBase.h"
#include "BulkheadGameState.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Character/Misc/EnemyRoute.h"
//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif

// Sets default values
AEnemySpawn::AEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnDirection"));
	ArrowComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
	ProjectGoatGameMode = Cast<AProjectGoatGameMode>(GetWorld()->GetAuthGameMode());
	ProjectGoatGameMode->OnStartCombatWave.AddDynamic(this, &AEnemySpawn::HandleOnCombatWaveStart);

	ProjectGoatGameState = Cast<ABulkheadGameState>(ProjectGoatGameMode->GameState);

	OnMiniWaveComplete.AddDynamic(this, &AEnemySpawn::HandleOnMiniWaveComplete);
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawn::HandleOnCombatWaveStart()
{
	CurrentMiniWaveIndex = 0;

	UE_LOG(LogTemp, Warning, TEXT("%s starts a wave"), *GetName());

	SpawnNextWave(CurrentWaveStat);
}

void AEnemySpawn::SpawnSingleEnemy()
{
	if (EnemyToBeSpawn)
	{
		int32 NextMonsterID = GetNextMonsterID();     //get next spawning monster's id

		AEnemyBase* NewMonster = ProjectGoatGameMode->SpawnMonster(NextMonsterID, ArrowComp->GetComponentLocation(), ArrowComp->GetComponentRotation());  //spawning monster

		NewMonster->BaseLocation = ProjectGoatGameMode->Base->GetActorLocation();    //set monster's base(destination) location

		NewMonster->NavPoints = GetNavPoints(EnemyToBeSpawn % CurrentRoutes.Num());  //set monster's nav points

		ProjectGoatGameState->AddActiveEnemy(NewMonster);      //add to global monster list

		ActiveEnemy.Add(NewMonster);                         //add to local monster list

		EnemyToBeSpawn--;
	}
	else
	{
		OnMiniWaveComplete.Broadcast();                      //if EnemyToBeSpawn == 0, all enemies had been spawned. 
		GetWorld()->GetTimerManager().ClearTimer(SpawningTimer);    //clear timer
	}
}

void AEnemySpawn::SpawnNextWave(const TArray<FMiniWaveDetail>& InWaveStat)
{
	if (CurrentMiniWaveIndex >= InWaveStat.Num())
	{
		OnWaveComplete.Broadcast();
	}

	if (InWaveStat.Num() > 0)
	{
		CurrentMiniWaveStat = InWaveStat[CurrentMiniWaveIndex];
		SpawnMiniWave(CurrentMiniWaveStat);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"),*GetName().Append(" weve stat invalid"));
	}
}

void AEnemySpawn::SpawnMiniWave(const FMiniWaveDetail& InMiniWave)
{
	ParseRouteNumber(InMiniWave.Route);

	EnemyToBeSpawn = InMiniWave.BossNumber + InMiniWave.GruntsNumber + InMiniWave.ReaversNumber;

	CurrentMiniWaveStat = InMiniWave;

	GetWorld()->GetTimerManager().SetTimer(SpawningTimer, this, &AEnemySpawn::SpawnSingleEnemy, SpawnInterval, true, 0.f);
}

/*
if a mini wave is complete, check if theres remaining mini wave.
if so, wait for delay then spawn another mini wave
else, broadcast OnWaveComplete. 
*/
void AEnemySpawn::HandleOnMiniWaveComplete()
{
	CurrentMiniWaveIndex++;
	if (CurrentMiniWaveIndex >= CurrentWaveStat.Num())   //all mini wave spawned
	{
		OnWaveComplete.Broadcast();
		CurrentWaveStat.Empty();
		for (auto& Tmp : Routes)
		{
			Tmp->SetSplineMeshesIsHidden(true);
		}
	}
	else
	{
		SetWaitTimer(0.5, CurrentMiniWaveStat.DelayToNextWave);
	}
}

bool AEnemySpawn::SetWaitTimer(const float& TickInterval, const float& TimerDuration)
{
	if (TimerTickCount > 0) //meaning timer is going
	{
		return false;
	}

	TimerTickCount = (int)TimerDuration / TickInterval;

	GetWorld()->GetTimerManager().SetTimer(WaitingTimer, this, &AEnemySpawn::WaitTimerTick, TickInterval, true, 0.f);

	return true;
}

void AEnemySpawn::WaitTimerTick()
{
	if (TimerTickCount)   //timer still ticking
	{
		TimerTickCount--;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitingTimer);
		if (CurrentMiniWaveIndex != CurrentWaveStat.Num())
		{
			SpawnMiniWave(CurrentWaveStat[CurrentMiniWaveIndex]);
		}
	}
}

void AEnemySpawn::DestroyWaveActor()
{
	for (AActor* Tmp : ActiveEnemy)
	{
		Tmp->Destroy();
	}
}

/*
get nav point for NavPointArray. 
if there's 3 enemy route under this spawn point, there will be 3 elements in NavPointArray
*/

TArray<FVector>& AEnemySpawn::GetNavPoints(const int32& Index)
{
	return Routes[Index]->NavPoints;
}

void AEnemySpawn::ParseRouteNumber(const FString& InRouteString)
{
	TArray<FString> Temp;
	InRouteString.ParseIntoArray(Temp, TEXT(","), true);

	CurrentRoutes.Empty();
	for (FString& Tmp : Temp)
	{
		CurrentRoutes.Add(FCString::Atoi(*Tmp));
	}

}

int32 AEnemySpawn::GetNextMonsterID() const 
{
	//UE_LOG(LogTemp, Warning, TEXT("EnemyToBeSpawn: %i, ReaverNumber: %i, BossNumber: %i"), EnemyToBeSpawn, CurrentMiniWaveStat.ReaversNumber, CurrentMiniWaveStat.BossNumber)
	if (CurrentMiniWaveStat.ReaversNumber || CurrentMiniWaveStat.BossNumber)
	{
		if (EnemyToBeSpawn <= CurrentMiniWaveStat.BossNumber)
		{
			return CurrentMiniWaveStat.BossID;
		}
		else
		{
			switch (EnemyToBeSpawn % 2)
			{
			case 0:
				return CurrentMiniWaveStat.GruntsID;
			case 1:
				return CurrentMiniWaveStat.ReaversID;
			}
		}
	}
	else
	{
		return CurrentMiniWaveStat.GruntsID;
	}

	return 0;
}

/*

*/
void AEnemySpawn::GetPossibleRoutesAndLight()
{
	TSet<int> PossibleRoutes;   

	for (auto& Tmp : CurrentWaveStat)       //look through all mini wave stats
	{
		TArray<FString> Temp;
		Tmp.Route.ParseIntoArray(Temp, TEXT(","), true);     //parse string

		for (FString& StringTmp : Temp)    // look through string route
		{
			int TempInt = FCString::Atoi(*StringTmp);
			if (!PossibleRoutes.Contains(TempInt))    //if this route is not in before, add
			{
				PossibleRoutes.Add(TempInt);
			}
		}
	}

	for (auto& Tmp : PossibleRoutes)
	{
		Routes[Tmp]->SetSplineMeshesIsHidden(false);
	}
}
//
//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif