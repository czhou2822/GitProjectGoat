// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectGoatGameMode.generated.h"

UCLASS(minimalapi)
class AProjectGoatGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AProjectGoatGameMode();
	
	UFUNCTION(BlueprintCallable)
	void SetCanBeBrittle(FGuid InID, bool result);

	UFUNCTION(BlueprintCallable)
	void SetIsBrittle(FGuid InID, bool result);
};



