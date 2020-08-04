// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BulkheadPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API ABulkheadPlayerState : public APlayerState
{
	GENERATED_BODY()


public:

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Gold;

	
};
