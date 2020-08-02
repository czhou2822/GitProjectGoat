// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BulkheadGameState.h"
#include "BulkheadPlayerState.h"
#include "ProjectGoatGameMode.generated.h"

//class ABulkheadGameState BulkheadGameState;
//class ABulkheadPlayerState BulkheadPlayerState;


UCLASS(minimalapi)
class AProjectGoatGameMode : public AGameMode
{
	GENERATED_BODY()
		
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABulkheadGameState* BulkheadGameState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABulkheadPlayerState* BulkheadPlayerState;





public:
	AProjectGoatGameMode();

	virtual void BeginPlay() override;
	
	void Init();
	
	/*Spawning*/
	ABulkheadCharacterBase* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, const ECharacterType& Type, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	template<class T>
	T* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, const ECharacterType& Type, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator)
	{
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, Type, Location, Rotator));
	}

	UFUNCTION(BlueprintCallable, Category = Spawn)
	AEnemyBase* SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ATowerBase* SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator = FRotator::ZeroRotator);



	/*Gameplay*/
	UFUNCTION(BlueprintCallable)
	void SetCanBeBrittle(FGuid InID, bool result);

	UFUNCTION(BlueprintCallable)
	void SetIsBrittle(FGuid InID, bool result);


	/*Gold*/
	UFUNCTION(BlueprintCallable)
	bool CollectGold(int InGold);

	UFUNCTION(BlueprintCallable)
	bool ConsumeGold(int InGold);


};



