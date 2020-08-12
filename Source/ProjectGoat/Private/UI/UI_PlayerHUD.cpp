// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_PlayerHUD.h"
#include "ProjectGoat/Public/BulkheadGameState.h"
#include "ProjectGoat/Public/BulkheadPlayerState.h"

void UUI_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	BulkheadPlayerState = Cast<ABulkheadPlayerState>(BulkheadGameState->PlayerArray[0]);
}