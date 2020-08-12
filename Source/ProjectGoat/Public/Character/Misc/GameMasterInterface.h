// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Misc/EnemySpawn.h"
#include "GameMasterInterface.generated.h"


/*
this is the interface for setting scene variable. e.g. enemy spawn, bIsDebugMode and such. 
ideally, this class does not contain any logic. 
*/

UCLASS()
class PROJECTGOAT_API AGameMasterInterface : public AActor
{
	GENERATED_BODY()

private:


public:

	/*Debug Params*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Params")
	bool StartGame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Params")
	bool IsDebug;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Params")
	UDataTable* WaveTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Params")
	UDataTable* DebugWaveTable;

	/*BuildingPhase*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Param")
	float BuildingPhaseWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Param")
	float BuildingPhaseTickInterval;

	/*BuildingToCombat*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Param")
	float BuildingToCombatWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Param")
	float BuildingToCombatTickInterval;

	/*PostCombat*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Param")
	float PostCombatWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Param")
	float PostCombatTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase Param")
	int32 WaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	AActor* Base;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int32 InitGold;
	
public:	
	// Sets default values for this actor's properties
	AGameMasterInterface();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemySpawn*> SpawnPointsArrayInterface;



};
