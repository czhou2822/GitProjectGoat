// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectGoatType.h"
#include "GameFramework/SaveGame.h"
#include "BulkheadSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UBulkheadSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "CPP|Save")
		FPlayerData PlayerData;

	UPROPERTY(BlueprintReadWrite, Category = "CPP|Save")
		FLevelData LevelData;
};
