// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectGoatType.h"
#include "Character/Misc/Trap.h"

#include "BulkheadPlayerState.h"
#include "BulkheadGameState.h"
#include "ProjectGoatGameMode.generated.h"

//class ABulkheadGameState BulkheadGameState;
//class ABulkheadPlayerState BulkheadPlayerState;


UCLASS(minimalapi)
class AProjectGoatGameMode : public AGameMode
{
	GENERATED_BODY()
		
public:

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnd, bool, bIsGameEndSuccess);

	//UPROPERTY(BlueprintAssignable, Category = "C++")
	//FOnGameEnd OnGameEnd;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EGamePhase, OutGamePhase);

	UPROPERTY(BlueprintAssignable, Category = "GamePhase")
	FOnPhaseChanged OnPhaseChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCombatWave);

	UPROPERTY(BlueprintAssignable, Category = "GamePhase")
	FOnStartCombatWave OnStartCombatWave;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerTick, int32, TickCount);

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnTimerTick OnTimerTick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABulkheadGameState* BulkheadGameState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABulkheadPlayerState* BulkheadPlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AGameMasterInterface* GM;

	/*Number of Spawn Points in scene*/
	int32 SpawnPointNumbers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EGamePhase GamePhase;

	int32 WaveNumber;

	FSpawnWaveDetail CurrentWaveDetail;

	TArray<class AEnemySpawn*> SpawnPointsArray;

	/*PhaseTimer*/
	float PhaseTickInterval;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PhaseTickCount;

	FTimerHandle PhaseTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ATrap* Base;

	bool bIsInitialized;

	bool bIsGameEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsGameBeaten;

public:
	AProjectGoatGameMode();

	virtual void BeginPlay() override;
	

	
	/*Spawning*/
	/*
	* must pass a FCharacterData to the funciton
	* if FCharacterData is valid (name != NAME_None), stick that to the new spawn
	* if FCharacterData is not valid, create a new one
	*/
	ABulkheadCharacterBase* SpawnCharacter(const int32& CharacterID, const ECharacterType& Type, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator, FCharacterData DefaultData = FCharacterData());

	template<class T>
	T* SpawnCharacter(const int32& CharacterID, const ECharacterType& Type, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator, FCharacterData DefaultData = FCharacterData())
	{
		return Cast<T>(SpawnCharacter(CharacterID, Type, Location, Rotator, DefaultData));
	}

	UFUNCTION(BlueprintCallable, Category = Spawn)
	AEnemyBase* SpawnMonster(const int32& CharacterID, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ATowerBase* SpawnTower(const int32& CharacterID, FCharacterData DefaultData, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = "CPP")
	void Init();

	void GetGMAndSpawnPointInScene();

	/*GM*/
	void ReadDataFromGM();

	/*Gameplay*/
	UFUNCTION(BlueprintCallable, Category = "CPP")
	void SetCanBeBrittle(FGuid InID, bool result);

	UFUNCTION(BlueprintCallable, Category = "CPP")
	void SetIsBrittle(FGuid InID, bool result);

	UFUNCTION(BlueprintCallable, Category = "CPP")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "CPP")
	void RestartBulkheadGame();

	/*Gold*/
	UFUNCTION(BlueprintCallable, Category = "CPP")
	void CollectGold(int InGold);

	UFUNCTION(BlueprintCallable, Category = "CPP")
	bool ConsumeGold(int InGold);

	UFUNCTION(BlueprintCallable, Category = "CPP")
	void SetGamePhase(const EGamePhase& InGamePhase);

	/*GamePhase*/
	UFUNCTION()
	void HandleOnPhaseChanged(EGamePhase InPhase);

	UFUNCTION()
	void HandleOnWaveComplete();

	/*GamePhaseTimer*/
	bool SetPhaseTimer(const float& TickInterval, const float& TimerDuration);

	void PhaseTimerTick();


	/*Start Phase*/
	void StartBuildingPhase();

	void StartBuildingToCombatPhase(const int32& InWaveNumber);

	void StartCombatPhase();

	void StartPostCombatPhase();


	/*BuildingToCombatPhase*/
	UFUNCTION(BlueprintCallable)
	FSpawnWaveDetail& GetAndSetWaveStat(const int32& inWaveNumber) const;

	UFUNCTION(BlueprintCallable)
	void ParseAndSetActiveSpawnPoints(const FSpawnWaveDetail& InDetail);

	void CheckIfGameEnd();

	UFUNCTION(BlueprintCallable, Category = "C++")
	void EndGame(const bool Success);

	/*
	called when its postcombat. postcombat will be terminate early if all enemy were killed
	*/
	void PostCombatCheck();



};



