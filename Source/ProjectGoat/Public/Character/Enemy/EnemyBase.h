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
	TArray<FVector> NavPoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		float DefaultMaxSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly)
		float SlowDownPercentage = 0.7f;

	FTimerHandle SlowTimer;

	float TimerTickInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly)
		float SlowedTime = 2.f;
	int32 SlowCount = 0;
	int32 MaxSlowCount = 6;
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
		void SlowDown();

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
	void HandleSlowDown();

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	FTimerHandle SlowListener;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlowStart();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlowEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFrozenStart();

	virtual void Dying();

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
