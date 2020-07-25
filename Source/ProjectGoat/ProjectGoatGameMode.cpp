// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectGoatGameMode.h"
#include "ProjectGoatCharacter.h"
#include "BulkheadGameState.h"
#include "UObject/ConstructorHelpers.h"

AProjectGoatGameMode::AProjectGoatGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}


	GameStateClass = ABulkheadGameState::StaticClass();


}

void AProjectGoatGameMode::SetCanBeBrittle(FGuid InID, bool result)
{
	if (ABulkheadGameState* InGameState = Cast<ABulkheadGameState>(GameState))
	{
		InGameState->GetCharacterData(InID).bCanBeBrittle = result;
	}

}

void AProjectGoatGameMode::SetIsBrittle(FGuid InID, bool result)
{
	if (ABulkheadGameState* InGameState = Cast<ABulkheadGameState>(GameState))
	{
		if (InGameState->GetCharacterData(InID).bCanBeBrittle == true) {
			InGameState->GetCharacterData(InID).bIsBrittle = result;
		}
	}

}