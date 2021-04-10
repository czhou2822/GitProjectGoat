// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "EnemyBase.generated.h"

UCLASS()
class PROJECTGOAT_API AEnemyBase : public ABulkheadCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GoldDropRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BrittleDamageRate = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BaseLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnScraps = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> NavPoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	~AEnemyBase();

	UPROPERTY(EditDefaultsOnly)
		float DefaultMaxSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly)
		float SlowDownPercentage = 0.7f;

	FTimerHandle FrozenTimer;

	float FrozenTimerTickInterval = 0.1f;

	//time it takes for this enemy to completly frozen up. Frozen effect increase linearly. Seconds. E.g. if set to 2, this enemy takes 2 seconds to completly frozen
	UPROPERTY(EditDefaultsOnly, Category = "Bulkhead | Frozen Variables")
	float FrozenTime = 2.f;

	//time that enemy remain current frozen state in seconds. e.g. if when frost cannon move away from an enemy, the frozen state will stay this amount of time until the frozen state decays.
	UPROPERTY(EditDefaultsOnly, Category = "Bulkhead | Frozen Variables")
	float FrozenPauseTime = 2.f;

	//whether this enemy is being frozen/shot by frost cannon
	UPROPERTY(BlueprintReadWrite)
	bool bIsFreezing = false;

	//current frozen meter
	int32 FrozenMeter = 0;
	//max frozen meter
	int32 MaxFrozenMeter = 0;
	//current frozen meter
	int32 FrozenPausedMeter = 0;
	//max frozen meter
	int32 MaxFrozenPausedMeter = 0;

	int32 TickCount = 0;

public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<FVector> NavPoints;

public:
	// Sets default values for this character's properties
	AEnemyBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BulkheadInit() override;

	UFUNCTION(BlueprintCallable)
		void FreezeStart();

	UFUNCTION(BlueprintCallable)
		void StartSlow();

	UFUNCTION(BlueprintCallable)
		void EndSlow();

	UFUNCTION(BlueprintCallable)
		void StartBrittle();

	UFUNCTION(BlueprintCallable)
		void EndBrittle();

	UFUNCTION(BlueprintCallable, Category = "C++")
		void SetAnimGlobalPlayrate(float InPlayRate);

	//UFUNCTION(BlueprintCallable, Category = "EnemyBase")
	//	void SetNavPoints(TArray<FVector> InPoints);

	UFUNCTION()
	void FrozenTimerTick();

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	FTimerHandle SlowListener;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlowStart();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlowEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFrozenStart();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFrozen(float FrozenAmount);

	virtual void MarkForDead() override;

	//void onOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		UAudioComponent* GMAudioComponent_EnemyBreath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWEnemyBreath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWEnemyFootStep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWEnemyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWEnemyDeath;


};
