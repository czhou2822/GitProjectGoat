// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class PROJECTGOAT_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void SlowDown();
	UFUNCTION(BlueprintCallable)
		void StartSlow();
	UFUNCTION(BlueprintCallable)
		void EndSlow();

	UFUNCTION()
		void HandleSlowDown();

	FTimerHandle slowListener;
	UFUNCTION(BlueprintImplementableEvent)
		void OnSlowStart();
	UFUNCTION(BlueprintImplementableEvent)
		void OnSlowEnd();
	UFUNCTION(BlueprintImplementableEvent)
		void OnFrozenStart();

	//void onOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
