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

AProjectGoatGameMode::AProjectGoatGameMode()
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
			Tmp->OnWaveComplete.AddDynamic(this, &AProjectGoatGameMode::HandleOnWaveComplete);
			SpawnPointsArray.Add(Tmp);
		}
	}
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

bool AProjectGoatGameMode::CollectGold(int InGold)
{
	if (BulkheadPlayerState)
	{
		BulkheadPlayerState->Gold += InGold;
		return true;
	}
	return false;
}

bool AProjectGoatGameMode::ConsumeGold(int InGold)
{
	if (BulkheadPlayerState)
	{
		if (BulkheadPlayerState->Gold < InGold)
		{
			return false;
		}
		BulkheadPlayerState->Gold -= InGold;
		return true;
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
		StartBuildingPhase();
		break;
	case EGamePhase::BUILDINGTOCOMBAT:
		WaveNumber++;
		StartBuildingToCombatPhase(WaveNumber);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
		break;
	case EGamePhase::COMBAT:
		break;
	case EGamePhase::POSTCOMBAT:
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
	if (PhaseTickCount >= 0) //meaning timer is going
	{
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
			break;
		}
	}
}

void AProjectGoatGameMode::StartBuildingPhase()
{
	SetPhaseTimer(GM->BuildingPhaseTickInterval, GM->BuildingPhaseWaitTime);
	BulkheadGameState->ClearActiveEnemyList();
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
		//UE_LOG(Debug, Warning, TEXT("spawn points invalids"));
		return;
	}

	for (int32 i = 0; i < InDetail.SpawnPoints.Num(); i++)
	{
		const FActiveSpawnPointStruct& Tmp = InDetail.SpawnPoints[i];
		SpawnPointsArray[Tmp.SpawnPointIndex]->CurrentWaveStat = Tmp.MiniWaveNumbers;
		SpawnPointsArray[Tmp.SpawnPointIndex]->SpawnInterval = Tmp.SpawnInterval;
	}
		
}


ABulkheadCharacterBase* AProjectGoatGameMode::SpawnCharacter(
	int32 CharacterID,
	int32 CharacterLevel,
	const ECharacterType& Type,
	const FVector& Location,
	const FRotator& Rotator)
{

	if (FCharacterData* NewCharacterData = BulkheadGameState->GetCharacterDataByID(CharacterID, Type))
	{
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


AEnemyBase* AProjectGoatGameMode::SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<AEnemyBase>(CharacterID, CharacterLevel, ECharacterType::MONSTER, Location, Rotator);
}

ATowerBase* AProjectGoatGameMode::SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<ATowerBase>(CharacterID, CharacterLevel, ECharacterType::TOWER, Location, Rotator);
}

void AProjectGoatGameMode::ReadDataFromGM()
{
	if (GM)
	{
		WaveNumber = GM->WaveNumber;
	}
}

