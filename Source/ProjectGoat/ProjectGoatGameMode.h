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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnd, bool, bIsGameEndSuccess);

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnGameEnd OnGameEnd;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EGamePhase, OutGamePhase);

	UPROPERTY(BlueprintAssignable, Category = "GamePhase")
	FOnPhaseChanged OnPhaseChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCombatWave);

	UPROPERTY(BlueprintAssignable, Category = "GamePhase")
	FOnStartCombatWave OnStartCombatWave;

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
	
	void Init();
	
	/*Spawning*/
	ABulkheadCharacterBase* SpawnCharacter(const int32& CharacterID, const ECharacterType& Type, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	template<class T>
	T* SpawnCharacter(const int32& CharacterID, const ECharacterType& Type, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator)
	{
		return Cast<T>(SpawnCharacter(CharacterID, Type, Location, Rotator));
	}

	UFUNCTION(BlueprintCallable, Category = Spawn)
	AEnemyBase* SpawnMonster(const int32& CharacterID, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ATowerBase* SpawnTower(const int32& CharacterID, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	/*GM*/
	void ReadDataFromGM();

	/*Gameplay*/
	UFUNCTION(BlueprintCallable)
	void SetCanBeBrittle(FGuid InID, bool result);

	UFUNCTION(BlueprintCallable)
	void SetIsBrittle(FGuid InID, bool result);

	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "C++")
	void RestartBulkheadGame();

	/*Gold*/
	UFUNCTION(BlueprintCallable)
	void CollectGold(int InGold);

	UFUNCTION(BlueprintCallable)
	bool ConsumeGold(int InGold);

	UFUNCTION(BlueprintCallable)
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
	void EndGame(const bool& Success);

	/*
	called when its postcombat. postcombat will be terminate early if all enemy were killed
	*/
	void PostCombatCheck();



};



