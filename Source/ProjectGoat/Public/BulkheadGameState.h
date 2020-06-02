// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/CharacterData.h"
#include "BulkheadGameState.generated.h"

class UDataTable;
class ABulkheadCharacterBase;
class AEnemyBase;

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

public:
	UPROPERTY(SaveGame)
	TMap<FGuid, FCharacterData> InGameEnemyData;
	
protected:

	ABulkheadCharacterBase* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator);

	template<class T>
	T* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator)
	{
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}

public:
	ABulkheadGameState();

	const FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData& Data);

	bool RemoveCharacterData(const FGuid& ID);

	FCharacterData& GetCharacterData(const FGuid& ID);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	AEnemyBase* SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);



};
