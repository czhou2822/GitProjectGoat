// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_BulkheadUIBase.h"
#include "Components/TextBlock.h"
#include "ProjectGoat/ProjectGoatType.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "UI_PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UUI_PlayerInfo : public UUI_BulkheadUIBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PGSnowMeter;

	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* TBSnowText;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UButton* TBMortar;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UButton* TBTesla;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	class UButton* TBGatling;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* GoldCount;

	TArray<UButton*> ButtonArray;


public:

	UUI_PlayerInfo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	UFUNCTION()
	void SelectTower(ETowerType InTower);

	UFUNCTION()
	void UpdateGold(int32 InGold);

	void ResetButtonOpacity();
};
