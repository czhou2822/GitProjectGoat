// Fill out your copyright notice in the Description page of Project Settings.


#include "BulkheadGameState.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Tower/TowerBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif


FCharacterData CharacterDataNULL;


ABulkheadGameState::ABulkheadGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> GruntTable(TEXT("/Game/DataTable/DataTable_Monster"));
	MonsterDataTable = GruntTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> TeslaTowerTable(TEXT("/Game/DataTable/DataTable_Tower"));
	TowerDataTable = TeslaTowerTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> WaveStructTable(TEXT("/Game/DataTable/DataTable_WaveStruct"));
	WaveStructDataTable = WaveStructTable.Object;

	CacheTowerData = ReadDataFromTable(TowerDataTable);

	CacheMonsterData = ReadDataFromTable(MonsterDataTable);

	CacheMonsterData;
}

void ABulkheadGameState::GetAllWaveStats()
{
	WaveData.Empty();
	if (WaveStructDataTable)
	{
		WaveStructDataTable->GetAllRows(TEXT("Character Data"), WaveData);
	}
}


const FCharacterData& ABulkheadGameState::AddCharacterData(const FGuid& ID, const FCharacterData& Data)
{
	return InGameEnemyData.Add(ID, Data);
}

bool ABulkheadGameState::RemoveCharacterData(const FGuid& ID)
{
	return (bool)InGameEnemyData.Remove(ID);
}

FCharacterData& ABulkheadGameState::GetCharacterData(const FGuid& ID)
{
	if (InGameEnemyData.Contains(ID))
	{
		return InGameEnemyData[ID];
	}
	return CharacterDataNULL;
}

TMap<int32, FCharacterData*> ABulkheadGameState::ReadDataFromTable(UDataTable* InUDataTable)
{
	TArray<FCharacterData*> TempArray;
	
	TMap<int32, FCharacterData*> OutTMap;

	TempArray.Empty();

	OutTMap.Empty();

	InUDataTable->GetAllRows(TEXT("Character Data"), TempArray);
	
	for (FCharacterData* Tmp : TempArray)
	{
		OutTMap.Add(Tmp->ID, Tmp);
	}

	return OutTMap;
}



FCharacterData* ABulkheadGameState::GetCharacterDataByID(const int32& ID, const ECharacterType& Type)
{
	switch (Type)
	{
	case ECharacterType::TOWER:
	{
		if (CacheTowerData.Contains(ID))
		{
			return CacheTowerData[ID];
		}
	}
	case ECharacterType::MONSTER:
	{
		if (CacheMonsterData.Contains(ID))
		{
			return CacheMonsterData[ID];
		}
	}
	}
	return nullptr;
}



#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif