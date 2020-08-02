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



public:
	UPROPERTY(SaveGame)
	TMap<FGuid, FCharacterData> InGameEnemyData;

	UPROPERTY(EditAnywhere)
	ABulkheadCharacterBase* SpawningPoint;

	UPROPERTY()
	UDataTable* MonsterDataTable;

	UPROPERTY()
	UDataTable* TowerDataTable;

	TMap<int32, FCharacterData*> CacheTowerData;

	TMap<int32, FCharacterData*> CacheMonsterData;

	TArray<FWaveStructData*> WaveData;
	
protected:



public:
	ABulkheadGameState();

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	const FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData& Data);

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	bool RemoveCharacterData(const FGuid& ID);

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	FCharacterData& GetCharacterData(const FGuid& ID);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	void GetAllWaveStats();

	TMap<int32, FCharacterData*> ReadDataFromTable(UDataTable* InUDataTable);

	FCharacterData* GetCharacterDataByID(const int32& ID, const ECharacterType& Type = ECharacterType::TOWER);



};
