// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGoat/ProjectGoatType.h"
#include "GameFramework/PlayerState.h"
#include "BulkheadPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API ABulkheadPlayerState : public APlayerState
{
	GENERATED_BODY()


private:
	
	int32 Gold;


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedTowerChanged, ETowerType, NewTower);

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnSelectedTowerChanged OnSelectedTowerChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, int32, CurrentGold);

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnGoldChanged OnGoldChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoldConsumeFailed);

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnGoldConsumeFailed OnGoldConsumeFailed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETowerType SelectedTower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FrostMeter;


public:

	ABulkheadPlayerState();

	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable, Category = "C++")
	void AddCoinToPlayer(int32 InGold);

	UFUNCTION(BlueprintCallable, Category = "C++")
	bool ConsumeCoin(int32 InGold);

	UFUNCTION(BlueprintCallable, Category = "C++")
	bool CanConsumeCoin(int32 InGold);

	UFUNCTION(BlueprintCallable, Category = "C++")
	int32& GetGold();

	UFUNCTION(BlueprintCallable, Category = "C++")
	void SetGold(int32 InGold);

	UFUNCTION(BlueprintCallable, Category = "C++")
	void SetTowerType(const ETowerType InTowerType);
	
	void NextTower();
};
