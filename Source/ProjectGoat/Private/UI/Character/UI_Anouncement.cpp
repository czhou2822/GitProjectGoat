// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/UI_Anouncement.h"
#include "Components/TextBlock.h"
#include "BulkheadPlayerState.h"


UUI_Anouncement::UUI_Anouncement(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUI_Anouncement::NativeConstruct()
{
	Super::NativeConstruct();

	BulkheadPlayerState->OnGoldConsumeFailed.AddDynamic(this, &UUI_Anouncement::StartAnouncement);

	TBTip->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Anouncement::StartAnouncement()
{
	StartTimer(0.5, 2);

	FString NewText = "Not Enough Money";

	TBTip->SetText(FText::FromString(NewText));
	TBTip->SetVisibility(ESlateVisibility::Visible);
}

void UUI_Anouncement::StartTimer(const float& TickInterval, const float& ShownTime)
{
	TimerCounter = ShownTime / TickInterval;

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UUI_Anouncement::AnouncementTick, TickInterval, true, 0.0f);
}

void UUI_Anouncement::AnouncementTick()
{
	if (TimerCounter)
	{
		TimerCounter--;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		TBTip->SetVisibility(ESlateVisibility::Hidden);
	}
}
