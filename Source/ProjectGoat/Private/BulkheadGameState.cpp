// Fill out your copyright notice in the Description page of Project Settings.


#include "BulkheadGameState.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/Enemy/EnemyBase.h"
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif


FCharacterData CharacterDataNULL;


ABulkheadGameState::ABulkheadGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Enemy(TEXT("/Game/DataTable/GruntDataTable"));

	GruntDataTable = MyTable_Enemy.Object;
}

ABulkheadCharacterBase* ABulkheadGameState::SpawnCharacter(
	int32 CharacterID,
	int32 CharacterLevel,
	UDataTable* InCharacterData,
	const FVector& Location,
	const FRotator& Rotator)
{
	if (InCharacterData)
	{
		TArray<FCharacterData*> Data;
		InCharacterData->GetAllRows(TEXT("Character Data"), Data);
		auto GetCharacterData = [&](int32 ID) -> FCharacterData*
		{
			for (auto& Tmp : Data)
			{
				if (Tmp->ID == ID)
				{
					return Tmp;
				}
			}
			return nullptr;
		};

		if (FCharacterData* NewCharacterData = GetCharacterData(CharacterID))
		{
			UClass* NewClass = NewCharacterData->CharacterBlueprintKey.LoadSynchronous();

			if (GetWorld() && NewClass)
			{
				if (ABulkheadCharacterBase* RuleOfTheCharacter = GetWorld()->SpawnActor<ABulkheadCharacterBase>(NewClass, Location, Rotator))
				{
					NewCharacterData->UpdateHealth();
					AddCharacterData(RuleOfTheCharacter->GUID, *NewCharacterData);
				}
			}
		}

	}

	return nullptr;
}


AEnemyBase* ABulkheadGameState::SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<AEnemyBase>(CharacterID, CharacterLevel, GruntDataTable, Location, Rotator);
}


const FCharacterData& ABulkheadGameState::AddCharacterData(const FGuid& ID, const FCharacterData& Data)
{
	return InGameEnemyData.Add(ID, Data);
}

bool ABulkheadGameState::RemoveCharacterData(const FGuid& ID)
{
	return (bool)InGameEnemyData.Remove(ID);
}

FCharacterData& ABulkheadGameState::GetCharacterData(const FGuid& ID)
{

	if (InGameEnemyData.Contains(ID))
	{
		InGameEnemyData[ID];
	}


	return CharacterDataNULL;
}


#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif