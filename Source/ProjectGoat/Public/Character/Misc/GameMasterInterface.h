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
public:

	/*BuildingPhase*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Phase")
	float BuildingPhaseWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Phase")
	float BuildingPhaseTickInterval;

	/*BuildingToCombat*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BuildingToCombat")
	float BuildingToCombatWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BuildingToCombat")
	float BuildingToCombatTickInterval;

	/*PostCombat*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PostCombat")
	float PostCombatWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PostCombat")
	float PostCombatTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNumber;

	
public:	
	// Sets default values for this actor's properties
	AGameMasterInterface();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemySpawn*> SpawnPointsArrayInterface;


};
