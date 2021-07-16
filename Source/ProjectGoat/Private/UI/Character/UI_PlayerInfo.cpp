// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/UI_PlayerInfo.h"
#include "BulkheadPlayerState.h"

//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif

UUI_PlayerInfo::UUI_PlayerInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ButtonArray.Empty();

}

void UUI_PlayerInfo::NativeConstruct()
{
	Super::NativeConstruct();

	BulkheadPlayerState->OnGoldChanged.AddDynamic(this, &UUI_PlayerInfo::UpdateGold);
	BulkheadPlayerState->OnFrostCannonChangedPercentage.AddDynamic(this, &UUI_PlayerInfo::UpdateSnowMeterPG);
	BulkheadPlayerState->OnSelectedTowerChanged.AddDynamic(this, &UUI_PlayerInfo::UpdateSelectedTower);


	UpdateGold(BulkheadPlayerState->GetGold());
}

void UUI_PlayerInfo::SelectTower(ETowerType InTower)
{
	ResetButtonOpacity();

}

void UUI_PlayerInfo::UpdateGold(int32 InGold)
{

	FString NewGoldCount = "";


	NewGoldCount.AppendInt(InGold);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *NewGoldCount);

	GoldCount->SetText(FText::FromString(NewGoldCount));


}

void UUI_PlayerInfo::UpdateSnowMeterPG(float InPercentage)
{
	PGSnowMeter->SetPercent(InPercentage);
	HandleOnSnowChangedPercentage(InPercentage);
}

void UUI_PlayerInfo::UpdateSelectedTower(ETowerType InTowerType)
{
	HandleOnSelectedTowerChanged(InTowerType);
}

void UUI_PlayerInfo::ResetButtonOpacity()
{
	for (UButton* Tmp : ButtonArray)
	{
		if (Tmp)
		{
			Tmp->SetRenderOpacity(0.3);
		}
	}
}

//
//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif