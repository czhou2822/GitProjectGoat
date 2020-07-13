// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:

	////////////////////////////////////////////////////////////////////////////////////////
	//blueprint instance
	UPROPERTY(EditDefaultsOnly, Category = "Table")
	TAssetSubclassOf<class ABulkheadCharacterBase> CharacterBlueprintKey;

	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attribute")
		FName Name;

	//which characyter is it
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attribute")
		int32 ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attribute")
		int32 Lv;

	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float MaxHealth;

	UPROPERTY()
		float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attribute")
		float Attack;

	//attack interval. e.g. 0.5 means attacks every 0.5 seconds. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attribute")
		float AttackRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attribute")
		float RunSpeed;

	//set to true once frozen, takes more damage from tower
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attribute")
	bool bIsBrittle;

	bool bCanBeBrittle;

	//false -> enemy, true -> tower
	bool bTeam;


public:
	FCharacterData();

	bool IsValid() const;

	void UpdateStats();


};

USTRUCT(BlueprintType)
struct FWaveStructData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 WaveNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 GruntNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 GruntID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 ReaverNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 ReaverID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 MiniBossNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 MiniBossID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FString RouteNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 GoldDropRate;


public:
	FWaveStructData();

};

//USTRUCT(BlueprintType)
//struct FEnemyData : public FCharacterData
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
//		float MaxHealth;
//
//	UPROPERTY()
//		float Health;
//
//
//public:
//	FEnemyData();
//
//	void UpdateHealth();
//};