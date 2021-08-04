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


//engine includes
#include "Kismet/GameplayStatics.h"

void UBulkheadGameInstance::Init()
{
	Super::Init();

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		BulkheadSaveGame = Cast< UBulkheadSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		BulkheadSaveGame = NewObject< UBulkheadSaveGame>();
	}

}

bool UBulkheadGameInstance::SaveGame()
{
	SavePlayerInfo();
	SaveLevelInfo();
	UGameplayStatics::SaveGameToSlot(BulkheadSaveGame, SaveSlotName, 0);
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
		BulkheadSaveGame->LevelData.WaveNumber = GameMode->WaveNumber;
	}

	SaveTowerInfo();

	return true;

}

bool UBulkheadGameInstance::LoadLevelInfo()
{

	LoadTowerInfo();

	return true;
}

bool UBulkheadGameInstance::SaveTowerInfo()
{
	auto GameState = GetWorld()->GetGameState<ABulkheadGameState>();

	//if (GameState)
	//{
	//	int TowerNumber = GameState->ActiveTowers.Num();

	//	if (TowerNumber)
	//	{
	//		BulkheadSaveGame->LevelData.TowerList.Empty();

	//		for (auto Tower : GameState->ActiveTowers)
	//		{
	//			if (Tower)
	//			{
	//				BulkheadSaveGame->LevelData.TowerList.Add(TPair<FTransform, FCharacterData>(Tower->GetActorTransform(), Tower->GetCharacterData()));
	//			}
	//		}
	//	}
	//}

	if (GameState)
	{
		int TowerNumber = GameState->ActiveTowers.Num();

		if (TowerNumber)
		{
			BulkheadSaveGame->LevelData.TowerDataList.Empty();
			BulkheadSaveGame->LevelData.TowerLocationList.Empty();

			for (auto Tower : GameState->ActiveTowers)
			{
				if (Tower)
				{
					BulkheadSaveGame->LevelData.TowerDataList.Add(Tower->GetCharacterData());
					BulkheadSaveGame->LevelData.TowerLocationList.Add(Tower->GetActorTransform());
				}
			}

		}
	}

	return true;
}

bool UBulkheadGameInstance::LoadTowerInfo()
{
	auto GameMode = GetWorld()->GetAuthGameMode<AProjectGoatGameMode>();
	if (BulkheadSaveGame && GameMode)
	{
		//for (auto Tower : BulkheadSaveGame->LevelData.TowerList)
		//{
		//	FTransform SpawnTransform = Tower.Key;
		//	FCharacterData SpawnData = Tower.Value;
		//	auto NewTower = GameMode->SpawnTower(SpawnData.ID, SpawnData, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator());
		//	if (NewTower)
		//	{
		//		auto NewAnimInstance = Cast<UAnimTowerBase>(NewTower->GetMesh()->GetAnimInstance());
		//		if (NewAnimInstance)
		//		{
		//			NewAnimInstance->bIsPlaced = true;
		//			NewAnimInstance->bIsConstructionComplete = true;
		//		}
		//	}
		//}

		for (int i = 0; i<BulkheadSaveGame->LevelData.TowerDataList.Num(); i++)
		{
			FTransform SpawnTransform = BulkheadSaveGame->LevelData.TowerLocationList[i];
			FCharacterData SpawnData = BulkheadSaveGame->LevelData.TowerDataList[i];
			auto NewTower = GameMode->SpawnTower(SpawnData.ID, SpawnData, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator());
			if (NewTower)
			{
				//auto NewAnimInstance = Cast<UAnimTowerBase>(NewTower->GetMesh()->GetAnimInstance());
				//if (NewAnimInstance)
				//{
				//	NewAnimInstance->bIsPlaced = true;
				//	NewAnimInstance->bIsConstructionComplete = true;
				//}
				NewTower->InternalTowerPlaced.Broadcast();
				NewTower->OnConstructionComplete.Broadcast();
			}
		}


	}

	return true;
}
