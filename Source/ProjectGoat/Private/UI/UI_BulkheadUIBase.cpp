// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_BulkheadUIBase.h"
#include "ProjectGoat/ProjectGoatGameMode.h"
#include "BulkheadPlayerState.h"
#include "BulkheadGameState.h"


UUI_BulkheadUIBase::UUI_BulkheadUIBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GUID = FGuid::NewGuid();



}

void UUI_BulkheadUIBase::NativeConstruct()
{
	Super::NativeConstruct();
	BulkheadGameState = GetGameState();

	ProjectGoatGameMode = GetWorld()->GetAuthGameMode<AProjectGoatGameMode>();

	BulkheadPlayerState = Cast<ABulkheadPlayerState>(BulkheadGameState->PlayerArray[0]);
}

ABulkheadGameState* UUI_BulkheadUIBase::GetGameState()
{
	if (GetWorld())
	{
		return GetWorld()->GetGameState<ABulkheadGameState>();
	}
	return nullptr;
}


