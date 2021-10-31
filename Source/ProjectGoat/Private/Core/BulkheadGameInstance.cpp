// Fill out your copyright notice in the Description page of Project Settings.

//user includes
#include "Core/BulkheadGameInstance.h"
#include "Core/BulkheadSaveGame.h"
#include "Character/Tower/TowerBase.h"
#include "Character/Core/AnimTowerBase.h"
#include "../../Public/BulkheadGameState.h"
#include "../../Public/BulkheadPlayerState.h"
#include "../../ProjectGoatGameMode.h"
#include "../../ProjectGoatType.h"
#include "Character/Misc/GameMasterInterface.h"


//engine includes
#include "Kismet/GameplayStatics.h"

void UBulkheadGameInstance::Init()
{
	Super::Init();

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		BulkheadSaveGame = Cast<UBulkheadSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		BulkheadSaveGame = NewObject<UBulkheadSaveGame>();
	}

	bIsLoaded = false;

}

bool UBulkheadGameInstance::SaveGame()
{
	SavePlayerInfo();
	SaveLevelInfo();
	UGameplayStatics::SaveGameToSlot(BulkheadSaveGame, SaveSlotName, 0);

	UE_LOG(LogTemp, Warning, TEXT("Game Saved!"));


	return true;
}

bool UBulkheadGameInstance::LoadGame()
{
	LoadPlayerInfo();
	LoadLevelInfo();
	return true;
}

bool UBulkheadGameInstance::SavePlayerInfo()
{
	auto PlayerState = Cast<ABulkheadPlayerState>(GetWorld()->GetGameState()->PlayerArray[0]);
	if (PlayerState && BulkheadSaveGame)
	{
		BulkheadSaveGame->PlayerData.FrostMeter = PlayerState->GetFrostMeter();
		BulkheadSaveGame->PlayerData.Gold = PlayerState->GetGold();
	}
	return true;
}
bool UBulkheadGameInstance::LoadPlayerInfo()
{
	if (BulkheadSaveGame)
	{
		auto PlayerState = Cast<ABulkheadPlayerState>(GetWorld()->GetGameState()->PlayerArray[0]);
		if (PlayerState)
		{
			PlayerState->SetFrostMeter(BulkheadSaveGame->PlayerData.FrostMeter);
			PlayerState->SetGold(BulkheadSaveGame->PlayerData.Gold);
		}

	}
	return true;
}

bool UBulkheadGameInstance::SaveLevelInfo()
{
	auto GameMode = GetWorld()->GetAuthGameMode<AProjectGoatGameMode>();
	if (GameMode)
	{
		BulkheadSaveGame->LevelData.WaveNumber = GameMode->WaveNumber - 1;
		BulkheadSaveGame->LevelData.BaseHP = GameMode->GM->Base->BaseHP;
	}

	SaveTowerInfo();

	return true;

}

bool UBulkheadGameInstance::LoadLevelInfo()
{
	LoadTowerInfo();

	auto GameMode = GetWorld()->GetAuthGameMode<AProjectGoatGameMode>();
	if (GameMode)
	{
		GameMode->WaveNumber = BulkheadSaveGame->LevelData.WaveNumber;
		GameMode->GM->Base->SetBaseHealth(BulkheadSaveGame->LevelData.BaseHP);
	}

	return true;
}

bool UBulkheadGameInstance::SaveTowerInfo()
{
	auto GameState = GetWorld()->GetGameState<ABulkheadGameState>();

	if (GameState)
	{
		TArray<AActor*> FoundTowers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATowerBase::StaticClass(), FoundTowers);

		BulkheadSaveGame->LevelData.TowerDataList.Empty();
		BulkheadSaveGame->LevelData.TowerLocationList.Empty();


		for (auto Tower : FoundTowers)
		{
			ATowerBase* newTower = Cast<ATowerBase>(Tower);
			if (newTower)
			{
				BulkheadSaveGame->LevelData.TowerDataList.Add(newTower->GetCharacterData());
				BulkheadSaveGame->LevelData.TowerLocationList.Add(newTower->GetActorTransform());
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Saving. TowerNumber: %i"),FoundTowers.Num());

	}


	//if (GameState)
	//{
	//	int TowerNumber = GameState->ActiveTowers.Num();

	//	if (TowerNumber)
	//	{
	//		BulkheadSaveGame->LevelData.TowerDataList.Empty();
	//		BulkheadSaveGame->LevelData.TowerLocationList.Empty();

	//		for (auto Tower : GameState->ActiveTowers)
	//		{
	//			if (Tower)
	//			{
	//				BulkheadSaveGame->LevelData.TowerDataList.Add(Tower->GetCharacterData());
	//				BulkheadSaveGame->LevelData.TowerLocationList.Add(Tower->GetActorTransform());
	//			}
	//		}

	//	}
	//}

	return true;
}

bool UBulkheadGameInstance::LoadTowerInfo()
{
	auto GameMode = GetWorld()->GetAuthGameMode<AProjectGoatGameMode>();
	if (BulkheadSaveGame && GameMode)
	{
		for (int i = 0; i<BulkheadSaveGame->LevelData.TowerDataList.Num(); i++)
		{
			FTransform SpawnTransform = BulkheadSaveGame->LevelData.TowerLocationList[i];
			FCharacterData SpawnData = BulkheadSaveGame->LevelData.TowerDataList[i];
			auto NewTower = GameMode->SpawnTower(SpawnData.ID, SpawnData, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator());
			if (NewTower)
			{
				NewTower->InternalTowerPlaced.Broadcast();
				NewTower->OnConstructionComplete.Broadcast();
			}
		}


	}

	return true;
}
