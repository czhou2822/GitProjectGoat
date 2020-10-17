// Fill out your copyright notice in the Description page of Project Settings.


#include "BulkheadGameState.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Tower/TowerBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif


FCharacterData CharacterDataNULL;


ABulkheadGameState::ABulkheadGameState()
	:ActiveEnemyCounts(0)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> GruntTable(TEXT("/Game/DataTable/DataTable_Monster"));
	MonsterDataTable = GruntTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> TeslaTowerTable(TEXT("/Game/DataTable/DataTable_Tower"));
	TowerDataTable = TeslaTowerTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> WaveStructTable(TEXT("/Game/DataTable/DataTable_WaveStruct"));
	WaveStructDataTable = WaveStructTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DebugWaveStructTable(TEXT("/Game/DataTable/DataTable_Debug"));
	DebugWaveStructDataTable = DebugWaveStructTable.Object;

	CacheTowerData = ReadDataFromTable(TowerDataTable);

	CacheMonsterData = ReadDataFromTable(MonsterDataTable);

}

void ABulkheadGameState::GetAllWaveStats(UDataTable* Input)
{

	TArray<FSpawnWaveDetail*> TempArray;
	TempArray.Empty();

	CacheSpawnWaveData.Empty();

	int32 index = 1;

	Input->GetAllRows(TEXT("Character Data"), TempArray);

	for (FSpawnWaveDetail* Tmp : TempArray)
	{
		CacheSpawnWaveData.Add(index, Tmp);
		index++;
	}

}


const FCharacterData& ABulkheadGameState::AddCharacterData(const FGuid& ID, const FCharacterData& Data)
{
	return InGameCharacterData.Add(ID, Data);
}

bool ABulkheadGameState::RemoveCharacterData(const FGuid& ID)
{
	return (bool)InGameCharacterData.Remove(ID);
}

FCharacterData& ABulkheadGameState::GetCharacterData(const FGuid& ID)
{
	if (InGameCharacterData.Contains(ID))
	{
		return InGameCharacterData[ID];
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

FCharacterData ABulkheadGameState::GetTowerDataByID(const int32& ID)
{
	return *GetCharacterDataByID(ID);
}

void ABulkheadGameState::AddActiveEnemy(AEnemyBase* InEnemy)
{
	if (InEnemy)
	{
		ActiveEnemies.Add(InEnemy);
		UpdateActiveEnemyCounts();
	}
}

void ABulkheadGameState::CheckActiveEnemy(AEnemyBase* InEnemy)
{
	if (!InEnemy)
	{
		DeleteActiveEnemy(InEnemy);
	}
}

void ABulkheadGameState::DeleteActiveEnemy(AEnemyBase* InEnemy)
{
	ActiveEnemies.Remove(InEnemy);
	UpdateActiveEnemyCounts();
}

//if return true->all enemy are cleared. 
bool ABulkheadGameState::CheckAllActiveEnemy()
{
	bool IsAllDead = false;
	for (AEnemyBase* Tmp : ActiveEnemies)
	{
		if (Tmp)
		{
			IsAllDead = true;
		}
		else
		{
			ActiveEnemies.Remove(Tmp);
		}
	}
	return !IsAllDead;    //if IsAllDead is true, meaning at least one EnemyBase is alive, return false (enemy are not cleared)
						  //only if all condition falls to else path, can IsAllDead stays false->return true(enemy are all cleared)
}

void ABulkheadGameState::ClearActiveEnemyList()
{
	DestoryAllEnemy();
	ActiveEnemies.Empty();
	UpdateActiveEnemyCounts();
}

void ABulkheadGameState::UpdateActiveEnemyCounts()
{
	ActiveEnemyCounts = ActiveEnemies.Num();
}


void ABulkheadGameState::AddToPrioritizedList(AEnemyBase* InEnemy)
{
	if (InEnemy)
	{
		PrioritizedEnemyList.Add(InEnemy);
	}
}

void ABulkheadGameState::DeleteFromPrioritizedList(AEnemyBase* InEnemy)
{
	if (InEnemy)
	{
		PrioritizedEnemyList.Remove(InEnemy);
	}
}

void ABulkheadGameState::CheckIfInDebug(bool DebugMode)
{
	if (DebugMode)
	{
		GetAllWaveStats(DebugWaveStructDataTable);
	}
	else
	{
		GetAllWaveStats(WaveStructDataTable);
	}
}

void ABulkheadGameState::DestoryAllEnemy()
{
	for (AEnemyBase* Tmp : ActiveEnemies)
	{
		if (Tmp && !Tmp->IsPendingKill())
		{
			Tmp->Destroy();
		}
	}
}



//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif