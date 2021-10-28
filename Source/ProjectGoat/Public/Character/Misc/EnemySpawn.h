// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGoat/ProjectGoatType.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

UCLASS()
class PROJECTGOAT_API AEnemySpawn : public AActor
{
	GENERATED_BODY()

private:

	class AProjectGoatGameMode* ProjectGoatGameMode;

	class ABulkheadGameState* ProjectGoatGameState;

	TArray<int32> CurrentRoutes;

	FTimerHandle SpawningTimer;

	FTimerHandle WaitingTimer;

	int32 TimerTickCount;

	//active enemy for THIS spawn point
	//TSet<class AEnemyBase*> ActiveEnemy;

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveComplete);

	UPROPERTY(BlueprintAssignable)
	FOnWaveComplete OnWaveComplete;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMiniWaveComplete);

	UPROPERTY(BlueprintAssignable)
	FOnMiniWaveComplete OnMiniWaveComplete;

	//inclues an array of mini wave. Again, a Wave could consist multiple mini waves
	TArray<FMiniWaveDetail> CurrentWaveStat;

	FMiniWaveDetail CurrentMiniWaveStat;

	int32 CurrentMiniWaveIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SpawnInterval;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 EnemyToBeSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GruntsLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ReaversLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MinotaursLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class AEnemyRoute*> Routes;



private:
	//determine next route using member variable EnemyToBeSpawn
	//if there are multiple routes, it picks the route 1 by 1
	int32 GetNextRoute();

public:	

	// Sets default values for this actor's properties
	AEnemySpawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnCombatWaveStart();

	UFUNCTION(BlueprintCallable)
	void SpawnSingleEnemy();

	UFUNCTION(BlueprintCallable)
	void SpawnNextWave(const TArray<FMiniWaveDetail>& WaveStat);

	UFUNCTION(BlueprintCallable)
	void SpawnMiniWave(const FMiniWaveDetail& InMiniWave);

	/*GamePhaseTimer*/
	bool SetWaitTimer(const float& TickInterval, const float& TimerDuration);

	void WaitTimerTick();

	TArray<FVector>& GetNavPoints(const int32& Index);

	void ParseRouteNumber(const FString& InRouteString);

	UFUNCTION(BlueprintCallable)
	void HandleOnMiniWaveComplete();

	int32 GetNextMonsterID();

	//find possible routes in this wave and light them up
	void GetPossibleRoutesAndLight();



	void HideAllEnemyRoute();

};
