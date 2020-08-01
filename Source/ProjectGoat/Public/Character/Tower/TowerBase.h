// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/BulkheadCharacterBase.h"
#include <stdbool.h>
#include "TowerBase.generated.h"


UCLASS()
class PROJECTGOAT_API ATowerBase : public ABulkheadCharacterBase
{
	GENERATED_BODY()
	
public:	

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Sets default values for this actor's properties
	ATowerBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleOnTowerPlaced();


};
