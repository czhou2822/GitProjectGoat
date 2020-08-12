// Fill out your copyright notice in the Description page of Project Settings.


#include "BulkheadPlayerState.h"
ABulkheadPlayerState::ABulkheadPlayerState()
	:Gold(0)
{
}

void ABulkheadPlayerState::BeginPlay()
{
	SelectedTower = ETowerType::GATLING;
}

void ABulkheadPlayerState::SetGold(int32 InGold)
{
	Gold = InGold;
	OnGoldChanged.Broadcast(Gold);
}


void ABulkheadPlayerState::AddCoinToPlayer(int32 InGold)
{
	if (InGold <= 0)
	{
		return;
	}
	SetGold(Gold + InGold);
}

bool ABulkheadPlayerState::ConsumeCoin(int32 InGold)
{
	if (InGold > Gold)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough Gold"));
		OnGoldConsumeFailed.Broadcast();
		return false;	
	}
	SetGold(Gold - InGold);
	return true;
}

int32& ABulkheadPlayerState::GetGold()
{
	return Gold;
}

void ABulkheadPlayerState::SetTowerType(const ETowerType& InTowerType)
{
	SelectedTower = InTowerType;
}

void ABulkheadPlayerState::NextTower()
{
	switch (SelectedTower)
	{
	case ETowerType::TESLA:
		SelectedTower = ETowerType::MORTAR;
		break;
	case ETowerType::MORTAR:
		SelectedTower = ETowerType::GATLING;
		break;
	case ETowerType::GATLING:
		SelectedTower = ETowerType::TESLA;
	}
	OnSelectedTowerChanged.Broadcast(SelectedTower);
}
