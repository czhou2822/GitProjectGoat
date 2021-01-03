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
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTowerFire);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionComplete);

	//UPROPERTY(BlueprintAssignable, Category = "C++")
	//FOnTowerFire TowerFire;

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnConstructionComplete OnConstructionComplete;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere, Category = "BoxCollision")
		UBoxComponent* TowerPadding;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RangeMeshC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
		FName FirePointName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
		TSet<ATowerBase*> OverlappedTower;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireInterval = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPlaced = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TowerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TowerRange;

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
	void HandleOnTowerPlaced();

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
};
