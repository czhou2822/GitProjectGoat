// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/CharacterData.h"
#include "ProjectGoat/ProjectGoatType.h"
#include "BulkheadGameState.generated.h"

class UDataTable;
class ABulkheadCharacterBase;
class AEnemyBase;
class ATowerBase;

extern FCharacterData CharacterDataNULL;


/**
 * 
 */
UCLASS()
class PROJECTGOAT_API ABulkheadGameState : public AGameState
{
	GENERATED_BODY()

private:


	UPROPERTY()
	UDataTable* WaveStructDataTable;

	UPROPERTY()
	UDataTable* DebugWaveStructDataTable;

	//used to store every live enemy in the scene
	TSet<AEnemyBase*> ActiveEnemies;


	//check if such enemy is invalid. e.g. pending death or already dead.
	void CheckActiveEnemy(AEnemyBase* InEnemy);

public:
	UPROPERTY(SaveGame)
	TMap<FGuid, FCharacterData> InGameEnemyData;

	UPROPERTY(EditAnywhere)
	ABulkheadCharacterBase* SpawningPoint;

	//used to store intermidiated data between .uasset data and CacheMonsterData
	UPROPERTY()
	UDataTable* MonsterDataTable;

	//used to store intermidiated data between .uasset data and CacheTowerData
	UPROPERTY()
	UDataTable* TowerDataTable;
	
	//used to Tower store data read from UTable
	TMap<int32, FCharacterData*> CacheTowerData;

	//used to monster store data read from UTable
	TMap<int32, FCharacterData*> CacheMonsterData;

	TMap<int32, FSpawnWaveDetail*> CacheSpawnWaveData;

	//used to store prioritized enemy. e.g. the tower should attack whichever target thats slowed by player. 
	//Only when these objects are out of range, can the tower attack the closest target within ITS range.
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	TSet<AEnemyBase*> PrioritizedEnemyList;


public:
	ABulkheadGameState();

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	const FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData& Data);

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	bool RemoveCharacterData(const FGuid& ID);

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	FCharacterData& GetCharacterData(const FGuid& ID);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	void GetAllWaveStats(UDataTable* Input);

	TMap<int32, FCharacterData*> ReadDataFromTable(UDataTable* InUDataTable);

	FCharacterData* GetCharacterDataByID(const int32& ID, const ECharacterType& Type = ECharacterType::TOWER);

	//used to add enemy into active list, should be called whenever an enemy is called. 
	UFUNCTION(BlueprintCallable, Category = "ActiveEnemy")
	void AddActiveEnemy(AEnemyBase* InEnemy);

	//delete such enemy from ActiveEnemyList. 
	UFUNCTION(BlueprintCallable, Category = "ActiveEnemy")
	void DeleteActiveEnemy(AEnemyBase* InEnemy);

	//check active enemy list. if all invalid (e.g. all dead), return true -> all enemy are cleared. 
	UFUNCTION(BlueprintCallable, Category = "ActiveEnemy")
	bool CheckAllActiveEnemy();

	//hard reset on ActiveEnemyList 
	UFUNCTION(BlueprintCallable, Category = "ActiveEnemy")
	void ClearActiveEnemyList();
	
	UFUNCTION(BlueprintCallable, Category = "PrioritizedList")
	void AddToPrioritizedList(AEnemyBase* InEnemy);

	UFUNCTION(BlueprintCallable, Category = "PrioritizedList")
	void DeleteFromPrioritizedList(AEnemyBase* InEnemy);

	void CheckIfInDebug(bool DebugMode);

	/*this function clears all active enemy. use is as the last resort. called when postcombat ends*/
	void DestoryAllEnemy();
};
