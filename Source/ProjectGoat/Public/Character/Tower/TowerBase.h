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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTowerFire);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionComplete);

	UPROPERTY(BlueprintAssignable, Category = "C++")
		FOnTowerFire TowerFire;

	UPROPERTY(BlueprintAssignable, Category = "C++")
		FOnConstructionComplete OnConstructionComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDecalComponent* Decal;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class USkeletalMeshComponent* Mesher;
	UPROPERTY(EditAnywhere, Category = "BoxCollision")
		class UBoxComponent* TowerPadding;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class AEnemyBase* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
		FName FirePointName;
	UPROPERTY(Category = "C++", EditAnywhere)
		TSet<ATowerBase*> OverlappedTower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextFire=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireInterval = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPlaced = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TowerDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWTeslaTowerConstruction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWTeslaTowerAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWGatlingTowerConstruction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWGatlingTowerAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWMortarTowerConstruction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWMortarTowerAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWMortarTowerImpact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWTowerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWTowerDestruction;

	class ABulkheadPlayerState* BulkheadPlayerState;
	class ABulkheadGameState* BulkheadGameState;
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

	void HandleOnCharacterStartPlacing(bool PlacingMode);

	void SetRangeVisibility(bool InVisibility);

	UFUNCTION(BlueprintCallable, Category = "C++")
	void HandleOnConstructionComplete();

	void TowerInit();

	UFUNCTION()
	virtual void HandleFireEvent();

	void OnConstructionCompleteEvent();
	void SetTargetActor();
	void FireTimer(float B);
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void SetRedOutLine(bool IsOutline);
	void PlayFireSound();
};
