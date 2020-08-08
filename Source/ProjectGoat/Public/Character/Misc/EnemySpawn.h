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
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveComplete);

	UPROPERTY(BlueprintAssignable)
	FOnWaveComplete OnWaveComplete;


	//inclues an array of mini wave. Again, a Wave is consists of multiple mini waves
	//UPROPERTY(BlueprintReadOnly)
	TArray<FMiniWaveDetail> CurrentWaveStat;

	//active mini wave 
	//UPROPERTY(BlueprintReadOnly)
	FMiniWaveDetail* CurrentMiniWaveStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SpawnInterval;

public:	

	// Sets default values for this actor's properties
	AEnemySpawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
