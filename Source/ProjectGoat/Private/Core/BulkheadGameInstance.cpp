// Fill out your copyright notice in the Description page of Project Settings.

//user includes
#include "Core/BulkheadGameInstance.h"
#include "Core/BulkheadSaveGame.h"
#include "../../Public/BulkheadGameState.h"
#include "../../Public/BulkheadPlayerState.h"
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
		BulkheadSaveGame->PlayerData.FrostMeter = PlayerState->FrostMeter;
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
			PlayerState->FrostMeter = BulkheadSaveGame->PlayerData.FrostMeter;
			PlayerState->SetGold(BulkheadSaveGame->PlayerData.Gold);
		}

	}
	return true;
}

bool UBulkheadGameInstance::SaveLevelInfo()
{


	return true;

}

bool UBulkheadGameInstance::LoadLevelInfo()
{
	return true;

}
