// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Misc/EnemySpawn.h"
#include "Engine/World.h"
#include "ProjectGoat/ProjectGoatGameMode.h"
#include "Character/Enemy/EnemyBase.h"
#include "BulkheadGameState.h"
#include "Components/ArrowComponent.h"
#include "Character/Misc/Trap.h"
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

		if (NextMonsterID)
		{
			AEnemyBase *NewMonster = ProjectGoatGameMode->SpawnMonster(NextMonsterID, ArrowComp->GetComponentLocation(), ArrowComp->GetComponentRotation()); //spawning monster
			
			if (NewMonster)
			{
				NewMonster->BaseLocation = ProjectGoatGameMode->Base->GetBaseNavPoint(); //set monster's base(destination) location

				NewMonster->NavPoints = GetNavPoints(GetNextRoute()); //set monster's nav points

				ProjectGoatGameState->AddActiveEnemy(NewMonster); //add to global monster list

				//ActiveEnemy.Add(NewMonster);                         //add to local monster list

				EnemyToBeSpawn--;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Monster ID"));
		}
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
		UE_LOG(LogTemp, Warning, TEXT("%s"),*GetName().Append(" wave stat invalid"));
	}
}

void AEnemySpawn::SpawnMiniWave(const FMiniWaveDetail& InMiniWave)
{

	ParseRouteNumber(InMiniWave.Route);

	EnemyToBeSpawn = InMiniWave.BossNumber + InMiniWave.GruntsNumber + InMiniWave.ReaversNumber;

	GruntsLeft= InMiniWave.GruntsNumber;

	ReaversLeft = InMiniWave.ReaversNumber;

	MinotaursLeft = InMiniWave.BossNumber;

	CurrentMiniWaveStat = InMiniWave;

	//TODO create variables to track how many of each enemy has been spawned

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

	}
	else
	{
		SetWaitTimer(0.5, CurrentMiniWaveStat.DelayToNextWave);
	}
}

int32 AEnemySpawn::GetNextRoute()
{

	if (CurrentRoutes.Num() == 1)
	{
		return CurrentRoutes[0];
	}
	else
	{
		if(CurrentRoutes.Num())
		{
			return CurrentRoutes[EnemyToBeSpawn % CurrentRoutes.Num()];
		}
		UE_LOG(LogTemp, Error, TEXT("Invalid Route"));
		return 0;
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

int32 AEnemySpawn::GetNextMonsterID()
{
	//UE_LOG(LogTemp, Warning, TEXT("EnemyToBeSpawn: %i, ReaverNumber: %i, BossNumber: %i"), EnemyToBeSpawn, CurrentMiniWaveStat.ReaversNumber, CurrentMiniWaveStat.BossNumber)
	//TODO this implementation doesn't work as intended
	if (GruntsLeft)
	{
		if (ReaversLeft)
		{
			switch (EnemyToBeSpawn % 2)
			{
			case 0:
				GruntsLeft--;
				return CurrentMiniWaveStat.GruntsID;
			case 1:
				ReaversLeft--;
				return CurrentMiniWaveStat.ReaversID;
			}
		}
		GruntsLeft--;
		return CurrentMiniWaveStat.GruntsID;
	}
	if (ReaversLeft)
	{
		ReaversLeft--;
		return CurrentMiniWaveStat.ReaversID;
	}
	MinotaursLeft--;
	return CurrentMiniWaveStat.BossID;
	// if (CurrentMiniWaveStat.ReaversNumber || CurrentMiniWaveStat.BossNumber)
	// {
	// 	if (EnemyToBeSpawn <= CurrentMiniWaveStat.BossNumber)
	// 	{
	// 		MinotaursLeft--;
	// 		return CurrentMiniWaveStat.BossID;
	// 	}
	// 	else
	// 	{
	// 		switch (EnemyToBeSpawn % 2)
	// 		{
	// 		case 0:
	// 			return CurrentMiniWaveStat.GruntsID;
	// 		case 1:
	// 			return CurrentMiniWaveStat.ReaversID;
	// 		}
	// 	}
	// }
	// else
	// {
	// 	return CurrentMiniWaveStat.GruntsID;
	// }

	// return 0;
}

/*
get possible routes and turn on the vfx
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
		Routes[Tmp]->ToggleRouteSegmentVFXInterface(true);
	}
}

void AEnemySpawn::HideAllEnemyRoute()
{
	for (auto& Tmp : Routes)
	{
		Tmp->SetSplineMeshesIsHidden(true);
		Tmp->ToggleRouteSegmentVFXInterface(false);
	}
}
//
//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif