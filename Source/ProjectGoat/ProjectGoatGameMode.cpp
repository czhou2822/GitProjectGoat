// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectGoatGameMode.h"
#include "ProjectGoatCharacter.h"
#include "BulkheadGameState.h"
#include "BulkheadPlayerState.h"
#include "Engine/World.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Tower/TowerBase.h"
#include "UObject/ConstructorHelpers.h"

AProjectGoatGameMode::AProjectGoatGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}


	GameStateClass = ABulkheadGameState::StaticClass();
	PlayerStateClass = ABulkheadPlayerState::StaticClass();
	

}

void AProjectGoatGameMode::BeginPlay()
{
	Super::BeginPlay();
	Init();
}



void AProjectGoatGameMode::Init()
{
	BulkheadGameState = Cast<ABulkheadGameState>(GetWorld()->GetGameState());
	BulkheadPlayerState = Cast<ABulkheadPlayerState>(GetWorld()->GetGameState()->PlayerArray[0]);
}

void AProjectGoatGameMode::SetCanBeBrittle(FGuid InID, bool result)
{
	if (ABulkheadGameState* InGameState = Cast<ABulkheadGameState>(GameState))
	{
		InGameState->GetCharacterData(InID).bCanBeBrittle = result;
	}

}

void AProjectGoatGameMode::SetIsBrittle(FGuid InID, bool result)
{
	if (ABulkheadGameState* InGameState = Cast<ABulkheadGameState>(GameState))
	{
		if (InGameState->GetCharacterData(InID).bCanBeBrittle == true) {
			InGameState->GetCharacterData(InID).bIsBrittle = result;
		}
	}

}

bool AProjectGoatGameMode::CollectGold(int InGold)
{
	if (BulkheadPlayerState)
	{
		BulkheadPlayerState->Gold += InGold;
		return true;
	}
	return false;
}

bool AProjectGoatGameMode::ConsumeGold(int InGold)
{
	if (BulkheadPlayerState)
	{
		if (BulkheadPlayerState->Gold < InGold)
		{
			return false;
		}
		BulkheadPlayerState->Gold -= InGold;
		return true;
	}
	return false;
}


ABulkheadCharacterBase* AProjectGoatGameMode::SpawnCharacter(
	int32 CharacterID,
	int32 CharacterLevel,
	const ECharacterType& Type,
	const FVector& Location,
	const FRotator& Rotator)
{

	if (FCharacterData* NewCharacterData = BulkheadGameState->GetCharacterDataByID(CharacterID, Type))
	{
		UClass* NewClass = NewCharacterData->CharacterBlueprintKey.LoadSynchronous();

		if (GetWorld() && NewClass)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ABulkheadCharacterBase* RuleOfTheCharacter = nullptr;
			RuleOfTheCharacter = GetWorld()->SpawnActor<ABulkheadCharacterBase>(NewClass, Location, Rotator, SpawnParam);
			if (RuleOfTheCharacter)
			{
				NewCharacterData->UpdateStats();
				RuleOfTheCharacter->ResetGUID();
				if (BulkheadGameState)
				{
					BulkheadGameState->AddCharacterData(RuleOfTheCharacter->GUID, *NewCharacterData);
					RuleOfTheCharacter->BulkheadInit();
					return RuleOfTheCharacter;
				}
			}
		}
	}

	

	return nullptr;
}


AEnemyBase* AProjectGoatGameMode::SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<AEnemyBase>(CharacterID, CharacterLevel, ECharacterType::MONSTER, Location, Rotator);
}

ATowerBase* AProjectGoatGameMode::SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator)
{
	return SpawnCharacter<ATowerBase>(CharacterID, CharacterLevel, ECharacterType::TOWER, Location, Rotator);
}

