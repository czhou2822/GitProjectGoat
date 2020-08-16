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

	BulkheadPlayerState->OnSelectedTowerChanged.AddDynamic(this, &UUI_PlayerInfo::SelectTower);
	BulkheadPlayerState->OnGoldChanged.AddDynamic(this, &UUI_PlayerInfo::UpdateGold);



	ButtonArray.Add(TBMortar);
	ButtonArray.Add(TBTesla);
	ButtonArray.Add(TBGatling);

	UpdateGold(BulkheadPlayerState->GetGold());
	SelectTower(BulkheadPlayerState->SelectedTower);
}

void UUI_PlayerInfo::SelectTower(ETowerType InTower)
{
	ResetButtonOpacity();

	switch (InTower)
	{
	case ETowerType::TESLA:
		TBTesla->SetRenderOpacity(1.0);
		//UE_LOG(LogTemp, Warning, TEXT("TESLA"));
		break;
	case ETowerType::MORTAR:
		TBMortar->SetRenderOpacity(1.0);
		//UE_LOG(LogTemp, Warning, TEXT("MORTAR"));
		break;
	case ETowerType::GATLING:
		//UE_LOG(LogTemp, Warning, TEXT("GATLING"));
		TBGatling->SetRenderOpacity(1.0);
		break;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("default"));
		break;
	}
}

void UUI_PlayerInfo::UpdateGold(int32 InGold)
{
	FString NewGoldCount = "Gold: ";

	NewGoldCount.AppendInt(InGold);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *NewGoldCount);

	GoldCount->SetText(FText::FromString(NewGoldCount));


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