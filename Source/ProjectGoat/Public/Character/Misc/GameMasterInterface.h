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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* Base;
	
public:	
	// Sets default values for this actor's properties
	AGameMasterInterface();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemySpawn*> SpawnPointsArrayInterface;



};
