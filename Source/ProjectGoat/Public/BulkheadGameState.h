// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/CharacterData.h"
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
	UDataTable* GruntDataTable;

	UPROPERTY()
	UDataTable* TeslaTowerDataTable;

	UPROPERTY()
	UDataTable* WaveStructDataTable;

public:
	UPROPERTY(SaveGame)
	TMap<FGuid, FCharacterData> InGameEnemyData;

	UPROPERTY(EditAnywhere)
	ABulkheadCharacterBase* SpawningPoint;

	TArray<FWaveStructData*> WaveData;
	
protected:

	ABulkheadCharacterBase* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	template<class T>
	T* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator)
	{
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}

public:
	ABulkheadGameState();

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	const FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData& Data);

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	bool RemoveCharacterData(const FGuid& ID);

	UFUNCTION(BlueprintCallable, Category = CharacterData)
	FCharacterData& GetCharacterData(const FGuid& ID);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	AEnemyBase* SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ATowerBase* SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	void GetAllWaveStats();





};
