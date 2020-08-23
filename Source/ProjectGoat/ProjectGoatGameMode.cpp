// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectGoatGameMode.h"
#include "ProjectGoatCharacter.h"
#include "BulkheadGameState.h"
#include "BulkheadPlayerState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Misc/GameMasterInterface.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Tower/TowerBase.h"
#include "Character/Misc/EnemySpawn.h"
#include "UObject/ConstructorHelpers.h"
//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif


AProjectGoatGameMode::AProjectGoatGameMode()
	:IsInitialized(false)
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}


	GameStateClass = ABulkheadGameState::StaticClass();
	PlayerStateClass = ABulkheadPlayerState::StaticClass();

	
	

}

void AProjectGoatGameMode::BeginPlay()
{
	Super::BeginPlay();

	Init();
	ReadDataFromGM();
	if (GM)
	{
		if (GM->StartGame)
		{
			StartGame();
		}
	}

}

void AProjectGoatGameMode::Init()
{
	BulkheadGameState = Cast<ABulkheadGameState>(GetWorld()->GetGameState());
	BulkheadPlayerState = Cast<ABulkheadPlayerState>(GetWorld()->GetGameState()->PlayerArray[0]);

	OnPhaseChanged.AddDynamic(this, &AProjectGoatGameMode::HandleOnPhaseChanged);

	{   //finding GM
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameMasterInterface::StaticClass(), FoundActors);

		for (AActor* TActor : FoundActors)
		{
			AGameMasterInterface* MyActor = Cast<AGameMasterInterface>(TActor);

			if (MyActor)
			{
				GM = MyActor;
			}
		}
	}


	if (GM && GM->SpawnPointsArrayInterface.Num())
	{
		for (AEnemySpawn* Tmp : GM->SpawnPointsArrayInterface)
		{
			if (Tmp)
			{
				Tmp->OnWaveComplete.AddDynamic(this, &AProjectGoatGameMode::HandleOnWaveComplete);
				SpawnPointsArray.Add(Tmp);
			}
		}
	}

	GamePhase = EGamePhase::UNDEFINE;
}

void AProjectGoatGameMode::SetCanBeBrittle(FGuid InID, bool result)
{
	if (ABulkheadGameState* InGameState = Cast<ABulkheadGameState>(GameState))
	{
		InGameState->GetCharacterData(InID).bCanBeBrittle = result;
	}

}

void AProjectGoatGameMode::SetIsBrittle(FGuid InID, bool result)
{
	if (ABulkheadGameState* InGameState = Cast<ABulkheadGameState>(GameState))
	{
		if (InGameState->GetCharacterData(InID).bCanBeBrittle == true) {
			InGameState->GetCharacterData(InID).bIsBrittle = result;
		}
	}

}

void AProjectGoatGameMode::StartGame()
{
	SetGamePhase(EGamePhase::BUILDINGPHASE);
}

void AProjectGoatGameMode::CollectGold(int InGold)
{
	if (BulkheadPlayerState)
	{
		BulkheadPlayerState->AddCoinToPlayer(InGold);
	}

}

bool AProjectGoatGameMode::ConsumeGold(int InGold)
{
	if (BulkheadPlayerState)
	{
		return BulkheadPlayerState->ConsumeCoin(InGold);
	}
	return false;
}

void AProjectGoatGameMode::SetGamePhase(const EGamePhase& InGamePhase)
{
	GamePhase = InGamePhase;
	OnPhaseChanged.Broadcast(InGamePhase);
}

/*
this is the main loop of the game. 
the game cycles from BuildingPhase->BuildingToCombatPhase->Combat->PostCombat->BuildingPhase...
*/
void AProjectGoatGameMode::HandleOnPhaseChanged(EGamePhase InPhase)
{
	switch (InPhase)
	{
	case EGamePhase::BUILDINGPHASE:
		WaveNumber++;
		StartBuildingPhase();
		UE_LOG(LogTemp, Warning, TEXT("wave %i Building Phase"), WaveNumber);
		break;

	case EGamePhase::BUILDINGTOCOMBAT:
		StartBuildingToCombatPhase(WaveNumber);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
		UE_LOG(LogTemp, Warning, TEXT("Building To Combat Phase"));
		break;

	case EGamePhase::COMBAT:
		StartCombatPhase();
		UE_LOG(LogTemp, Warning, TEXT("Combat Phase"));
		break;

	case EGamePhase::POSTCOMBAT:
		StartPostCombatPhase();
		UE_LOG(LogTemp, Warning, TEXT("Post Combat Phase"));
		break;

	case EGamePhase::UNDEFINE:
		break;
	}
}

/*
since the GM knows the number of spawn points
whenever a spawn dispatch an "wave complete" event, 
GM substract from the total number of spawn points
once it reaches 0, all spawn points have finished a wave
hence the game move on to post-combat phase.
*/
void AProjectGoatGameMode::HandleOnWaveComplete()
{
	SpawnPointNumbers--;
	if (!SpawnPointNumbers)   
	{
		SetGamePhase(EGamePhase::POSTCOMBAT);
	}
}

bool AProjectGoatGameMode::SetPhaseTimer(const float& TickInterval, const float& TimerDuration)
{
	if (PhaseTickCount > 0) //meaning timer is going
	{
		return false;
	}

	if (TickInterval == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TickInterval 0, WaveNumber: %i, GamePhase: %i"), WaveNumber, GamePhase);
		return false;
	}

	PhaseTickCount = (int)TimerDuration / TickInterval;

	GetWorld()->GetTimerManager().SetTimer(PhaseTimerHandle, this, &AProjectGoatGameMode::PhaseTimerTick, TickInterval, true, 0.f);

	return true;
}

void AProjectGoatGameMode::PhaseTimerTick()
{
	if (PhaseTickCount)   //timer still ticking
	{
		if (GamePhase == EGamePhase::POSTCOMBAT)
		{
			PostCombatCheck();
		}
		PhaseTickCount--;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PhaseTimerHandle);   //if timer done, move on to next phase.
		switch (GamePhase)
		{
		case EGamePhase::BUILDINGPHASE:
			SetGamePhase(EGamePhase::BUILDINGTOCOMBAT);
			break;
		case EGamePhase::BUILDINGTOCOMBAT:
			SetGamePhase(EGamePhase::COMBAT);
			break;
		case EGamePhase::COMBAT:
			SetGamePhase(EGamePhase::POSTCOMBAT);
			break;
		case EGamePhase::POSTCOMBAT:
			SetGamePhase(EGamePhase::BUILDINGPHASE);
			break;
		case EGamePhase::UNDEFINE:
			SetGamePhase(EGamePhase::BUILDINGPHASE);
			break;
		}
	}
}

void AProjectGoatGameMode::StartBuildingPhase()
{
	if (!GM)
	{
		UE_LOG(LogTemp, Warning, TEXT("GM NULL. TickInterval 0, WaveNumber: %i, GamePhase: %i"), WaveNumber, GamePhase);
		return;
	}
	if (GM->SWWaveStart)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), GM->SWWaveStart);    //shot 1 time off sound effect
	}
	CheckIfGameEnd();
	SetPhaseTimer(GM->BuildingPhaseTickInterval, GM->BuildingPhaseWaitTime);
//	BulkheadGameState->ClearActiveEnemyList();   //has bug,read memory vialation
}

void AProjectGoatGameMode::StartBuildingToCombatPhase(const int32& InWaveNumber)
{
	if (GM)
	{
		SpawnPointNumbers = GM->SpawnPointsArrayInterface.Num();
		CurrentWaveDetail = GetAndSetWaveStat(InWaveNumber);
		ParseAndSetActiveSpawnPoints(CurrentWaveDetail);
		SetPhaseTimer(GM->BuildingToCombatTickInterval, GM->BuildingToCombatWaitTime);
	}
}

void AProjectGoatGameMode::StartCombatPhase()
{
	OnStartCombatWave.Broadcast();
}

void AProjectGoatGameMode::StartPostCombatPhase()
{
	SetPhaseTimer(GM->PostCombatTickInterval, GM->PostCombatWaitTime);
}

FSpawnWaveDetail& AProjectGoatGameMode::GetAndSetWaveStat(const int32& inWaveNumber) const
{
	return *BulkheadGameState->CacheSpawnWaveData[inWaveNumber];
}

void AProjectGoatGameMode::ParseAndSetActiveSpawnPoints(const FSpawnWaveDetail& InDetail)
{

	if (InDetail.SpawnPoints.Num() > GM->SpawnPointsArrayInterface.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("spawn points invalids. Spawn Points: %i, Points in scene: %i"), InDetail.SpawnPoints.Num(), GM->SpawnPointsArrayInterface.Num());
		return;
	}

	for (int32 i = 0; i < InDetail.SpawnPoints.Num(); i++)
	{
		const FActiveSpawnPointStruct& Tmp = InDetail.SpawnPoints[i];
		SpawnPointsArray[Tmp.SpawnPointIndex]->CurrentWaveStat = Tmp.MiniWaveNumbers;
		SpawnPointsArray[Tmp.SpawnPointIndex]->SpawnInterval = Tmp.SpawnInterval;
	}
		
}

void AProjectGoatGameMode::CheckIfGameEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("check game end %i, CacheSpawnWaveData.Num() %i"), WaveNumber, BulkheadGameState->CacheSpawnWaveData.Num());
	if (WaveNumber > BulkheadGameState->CacheSpawnWaveData.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("game ended. "));
		EndGame(true);
	}
}

void AProjectGoatGameMode::EndGame(const bool& Success)
{
	UE_LOG(LogTemp, Warning, TEXT("end game function"));
	if (Success) //if game ended success
	{
		UE_LOG(LogTemp, Warning, TEXT("game ended success "));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/EndGameLevel"), true);
	}

}

void AProjectGoatGameMode::PostCombatCheck()
{
	if (BulkheadGameState)
	{
		if (BulkheadGameState->CheckAllActiveEnemy())
		{
			GetWorld()->GetTimerManager().ClearTimer(PhaseTimerHandle);   //if timer done, move on to next phase.
			SetGamePhase(EGamePhase::BUILDINGPHASE);
		}
	}
}


ABulkheadCharacterBase* AProjectGoatGameMode::SpawnCharacter(
	const int32& CharacterID,
	const ECharacterType& Type,
	const FVector& Location,
	const FRotator& Rotator)
{
	if (FCharacterData* NewCharacterData = BulkheadGameState->GetCharacterDataByID(CharacterID, Type))
	{
		if (Type == ECharacterType::TOWER)
		{
			if (!BulkheadPlayerState->ConsumeCoin(NewCharacterData->Gold))  //not enough money
			{
				return nullptr;
			}
		}
		UClass* NewClass = NewCharacterData->CharacterBlueprintKey.LoadSynchronous();

		if (GetWorld() && NewClass)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ABulkheadCharacterBase* RuleOfTheCharacter = nullptr;
			RuleOfTheCharacter = GetWorld()->SpawnActor<ABulkheadCharacterBase>(NewClass, Location, Rotator, SpawnParam);
			if (RuleOfTheCharacter)
			{
				NewCharacterData->UpdateStats();
				RuleOfTheCharacter->ResetGUID();
				if (BulkheadGameState)
				{
					BulkheadGameState->AddCharacterData(RuleOfTheCharacter->GUID, *NewCharacterData);
					RuleOfTheCharacter->BulkheadInit();
					return RuleOfTheCharacter;
				}
			}
		}
	}

	

	return nullptr;
}


AEnemyBase* AProjectGoatGameMode::SpawnMonster(const int32& CharacterID, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<AEnemyBase>(CharacterID, ECharacterType::MONSTER, Location, Rotator);
}

ATowerBase* AProjectGoatGameMode::SpawnTower(const int32& CharacterID, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<ATowerBase>(CharacterID, ECharacterType::TOWER, Location, Rotator);
}

void AProjectGoatGameMode::ReadDataFromGM()
{
	if (GM)
	{
		WaveNumber = GM->WaveNumber;
		Base = GM->Base;
		BulkheadGameState->CheckIfInDebug(GM->IsDebug);
		BulkheadPlayerState->AddCoinToPlayer(GM->InitGold);
		UE_LOG(LogTemp, Warning, TEXT("Added Coins %i"), GM->InitGold);
	}
}

//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif
