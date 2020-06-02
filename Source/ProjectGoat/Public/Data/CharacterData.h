// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:

	////////////////////////////////////////////////////////////////////////////////////////
	//blueprint instance
	UPROPERTY(EditDefaultsOnly, Category = "Table")
	TAssetSubclassOf<class ABulkheadCharacterBase> CharacterBlueprintKey;

	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attribute")
		FName Name;

	//which characyter is it
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Attribute")
		int32 ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attribute")
		int32 Lv;

	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float MaxHealth;

	UPROPERTY()
		float Health;



public:
	FCharacterData();

	bool IsValid() const;

	void UpdateHealth();


};

//USTRUCT(BlueprintType)
//struct FEnemyData : public FCharacterData
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
//		float MaxHealth;
//
//	UPROPERTY()
//		float Health;
//
//
//public:
//	FEnemyData();
//
//	void UpdateHealth();
//};