// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_BulkheadUIBase.h"
#include "UI_Anouncement.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UUI_Anouncement : public UUI_BulkheadUIBase
{
	GENERATED_BODY()

private:
	
	int32 TimerCounter;

	FTimerHandle Timer;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TBTip;


public:

	UUI_Anouncement(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	UFUNCTION()
	void StartAnouncement();

	void StartTimer(const float& TickInterval, const float& ShownTime);

	void AnouncementTick();
};
