// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ProjectGoatType.generated.h"




UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	//descript what phase the game is in

	BUILDINGPHASE,     //Building phase, design to building buildings and collect resources 
	BUILDINGTOCOMBAT,  //setting wave stat and other things before combat, plays combat cue
	COMBAT,            //combat phase
	POSTCOMBAT,        //gives player time to finish/kill all enemy on the scene
	UNDEFINE           //default phase -> something went wrong
};


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	TOWER,
	MONSTER,
};


UENUM(BlueprintType)
enum class ETowerType : uint8
{
	TESLA,
	GATLING,
	MORTAR
};




/*
struct that specifies detail info for each mini wave. 
@param MiniWaveNumbers: which mini wave 
@param Route: which route this enemy takes. If more than 1 route, Reavers and Grunts spawns interchangeably, Miniboss always spawn the last.
@param GruntsID: stat of Grunt 
@param GruntsNumber: # of Grunts
@param ReaversID: stat of Reavers 
@param ReversNumber: # of Reavers spawned this mini wave. 
@param BossID: stat of Boss
@param BossNumber: # of boss spawned t his mini wave;
@param DelayToNextWave: delays in seconds between two mini wave. 
*/
USTRUCT(BlueprintType)
struct FMiniWaveDetail : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 MiniWaveNumbers;

	UPROPERTY(EditDefaultsOnly)
	FString Route;

	UPROPERTY(EditDefaultsOnly)
	float DelayToNextWave;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Stat")
	int32 GruntsID;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Stat")
	int32 GruntsNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Stat")
	int32 ReaversID;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Stat")
	int32 ReaversNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Stat")
	int32 BossID;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Stat")
	int32 BossNumber;
};

/*
this struct specifies wave info for a SINGLE spawn point. especially how many mini waves are in this wave. 
SpawnPointIndex: telling the GM which SpawnPoint is this, an ID to the system. 
SpawnInterval: time in seconds between 2 single enemies were spawned
MiniWaveNumber: this is an array, specifing how many mini wave were in this wave. 

*/
USTRUCT(BlueprintType)
struct FActiveSpawnPointStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 SpawnPointIndex;

	UPROPERTY(EditDefaultsOnly)
	float SpawnInterval;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMiniWaveDetail> MiniWaveNumbers;	
};

/*
struct that specifies what wave it is, and which spawning is participating this wave.
wave info such as # of enemies and enemy types, are specified in MiniWaveDetail.
A wave can have multiple mini waves.

@param WaveNumber: which wave it is
@param SpawnPoints: which spawn point spawns enemy in this wave.
*/
USTRUCT(BlueprintType)
struct FSpawnWaveDetail : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 WaveNumber;

	UPROPERTY(EditDefaultsOnly)
	TArray<FActiveSpawnPointStruct> SpawnPoints;
};