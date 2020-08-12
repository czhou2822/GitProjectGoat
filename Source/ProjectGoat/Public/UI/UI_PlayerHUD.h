// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Character/UI_PlayerInfo.h"
#include "UI/UI_BulkheadUIBase.h"
#include "UI_PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UUI_PlayerHUD : public UUI_BulkheadUIBase
{
	GENERATED_BODY()
	

public:
	UPROPERTY(meta = (BindWidget))
	class UUI_Anouncement* AnouncementSystem;

	UPROPERTY(meta = (BindWidget))
	class UUI_PlayerInfo* PlayerInfoSystem;


	class ABulkheadPlayerState* BulkheadPlayerState;
public:
	virtual void NativeConstruct();

};
