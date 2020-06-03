// Fill out your copyright notice in the Description page of Project Settings.


#include "BulkheadGameState.h"
#include "UI\Core\UI_BulkheadUIBase.h"

UUI_BulkheadUIBase::UUI_BulkheadUIBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GUID = FGuid::NewGuid();
}



ABulkheadGameState* UUI_BulkheadUIBase::GetGameState()
{
	if (GetWorld())
	{
		return GetWorld()->GetGameState<ABulkheadGameState>();
	}
	return nullptr;
}
