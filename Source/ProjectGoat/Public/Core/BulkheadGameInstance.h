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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnds, bool, bDidGameEndWithSuccess);

	UPROPERTY(BlueprintAssignable, Category = "CPP")
	FOnGameEnds OnGameEnds;

	UPROPERTY(BlueprintReadWrite, Category = "CPP | Save")
	class UBulkheadSaveGame* BulkheadSaveGame = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CPP | Save")
	FString SaveSlotName {};

	UPROPERTY(BlueprintReadWrite, Category = "CPP | Save")
	bool bIsLoaded = false;

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

	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool SaveTowerInfo();
	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		bool LoadTowerInfo();

	UFUNCTION(BlueprintCallable, Category = "CPP | Save")
		void DeleteGameSave();

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void BoardcastGameEnds(bool DidGameEndSuccess);


};
