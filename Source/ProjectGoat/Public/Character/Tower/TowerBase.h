// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "Delegates/Delegate.h"
#include "Components/BoxComponent.h"
#include "TowerBase.generated.h"



UCLASS()
class PROJECTGOAT_API ATowerBase : public ABulkheadCharacterBase
{
	GENERATED_BODY()
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTowerFire);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionComplete);

	UPROPERTY(BlueprintAssignable, Category = "Fire")
		FOnTowerFire TowerFire;
	UPROPERTY(BlueprintAssignable, Category = "TowerConstruction")
		FOnConstructionComplete OnConstructionComplete;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UDecalComponent* Decal;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMeshComponent* Mesher;
	UPROPERTY(VisibleAnywhere, Category = "BoxCollision")
		class UBoxComponent* TowerPadding;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AEnemyBase* TargetActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector FirePoint;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "FirePoint")
		FName FirePointName;
	UPROPERTY(Category = "OverlappedSet", EditAnywhere)
		TSet<ATowerBase*> OverlappedTower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NextFire=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireInterval = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPlaced = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Sets default values for this actor's properties
	ATowerBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleOnTowerPlaced();
	void HandleOnCharacterStartPlacing(bool PlacingMode);
	void SetRangeVisibility(bool InVisibility);
	void HandleOnConstructionComplete();
	void TowerInit();
	void FireEvent();
	void OnConstructionCompleteEvent();
	void SetTargetActor();
	void FireTimer(float B);
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void SetRedOutLine(bool IsOutline);
};
