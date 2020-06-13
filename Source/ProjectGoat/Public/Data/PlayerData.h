// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"



USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_USTRUCT_BODY()

	FPlayerData();

	void Init();

	bool IsValid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GameGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RocketCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxRocketCount;

};



