// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_BulkheadUIBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UUI_BulkheadUIBase : public UUserWidget
{
	GENERATED_BODY()
protected:

	class ABulkheadGameState* BulkheadGameState;

	class AProjectGoatGameMode* ProjectGoatGameMode;

	class ABulkheadPlayerState* BulkheadPlayerState;

public:

	UUI_BulkheadUIBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	FGuid GUID;

	class ABulkheadGameState* GetGameState();

	virtual void NativeConstruct();

};
