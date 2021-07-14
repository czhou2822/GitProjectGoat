// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BulkheadGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UBulkheadGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "CPP | Save")
	class UBulkheadSaveGame* BulkheadSaveGame = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CPP | Save")
	FString SaveSlotName {};



public:
	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool SaveGame();
	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool LoadGame();

	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool SavePlayerInfo();
	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool LoadPlayerInfo();

	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool SaveLevelInfo();
	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool LoadLevelInfo();
};
