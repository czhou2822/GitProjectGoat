// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "Delegates/Delegate.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundWave.h"
#include "TowerBase.generated.h"



UCLASS()
class PROJECTGOAT_API ATowerBase : public ABulkheadCharacterBase
{
	GENERATED_BODY()
	
private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInternalTowerPlaced);  //only used for this class, pass OnTowerPlaced from tpscharacter

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FInternalTowerPlaced InternalTowerPlaced;

public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionComplete);

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnConstructionComplete OnConstructionComplete;

	UPROPERTY(VisibleAnywhere, Category = "BoxCollision")
		UBoxComponent* TowerPadding;

	UPROPERTY(VisibleAnywhere, Category = "BoxCollision")
		class UAnimTowerBase* TowerAnim = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RangeMeshC;

	UPROPERTY(BlueprintReadWrite)
	bool bCanBeBuilt = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
		FName FirePointName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
		TSet<ATowerBase*> OverlappedTower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
		TSet<AEnemyBase*> EnemyList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireInterval = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPlaced = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TowerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TowerRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++|TowerTurning")
		float TurnSpeed = 0.2f;

	/*
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++|TowerTurning")
		float FacingTolrence = 100.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "C++")
		bool bIsAlignWithTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundBase* SWTowerAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundBase* SWTowerImpact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundBase* SWTowerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundBase* SWTowerDestruction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
		int32 SelfTowerID;

	class ABulkheadPlayerState* BulkheadPlayerState;

	class ABulkheadGameState* BulkheadGameState;

	FTimerHandle FireTimerHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
		AEnemyBase* TargetEnemy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Sets default values for this actor's properties
	ATowerBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "C++")
	void HandleOnTowerPlaced(ATowerBase* SpawnedTower);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "C++")
	void TowerFire();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "C++")
	void TowerFireTickInterface();

	UFUNCTION()
	void HandleOnCharacterStartPlacing(bool PlacingMode);

	void SetRangeVisibility(bool InVisibility);

	UFUNCTION(BlueprintCallable, Category = "C++")
	void HandleOnConstructionComplete();

	void TowerInit();

	virtual void BulkheadInit() override;

	void OnConstructionCompleteEvent();

	void SetTargetActor();

	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetRedOutLine(bool IsOutline);

	UFUNCTION(BlueprintCallable)
	void PlayFireSound();

	UFUNCTION(BlueprintCallable)
	float& GetTowerRange();

	UFUNCTION()
	void FireTimerTick();

	/*
	* decides how many degrees that the tower doom should turn in order to face the enemy
	*/
	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void TurnFirePointToTheEnemy();

	/*
	* decides how many degrees that the gun barrel, if applied, should pitch in order to face the enemy(for gattling) and fire the cannon(for mortar)
	*/
	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void PitchFirePointToEnemy();


};
