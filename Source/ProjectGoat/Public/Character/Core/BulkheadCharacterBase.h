// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BulkheadGameState.h"
#include "Data/CharacterData.h"
#include "BulkheadCharacterBase.generated.h"

UCLASS()
class PROJECTGOAT_API ABulkheadCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	FGuid GUID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//virtual FCharacterData& GetCharacterData();

	// Sets default values for this character's properties
	ABulkheadCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FCharacterData& GetCharacterData();

	FORCEINLINE ABulkheadGameState* GetGameState() { return GetWorld() ? GetWorld()->GetGameState<ABulkheadGameState>() : NULL; }

};
