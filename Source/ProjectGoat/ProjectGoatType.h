// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGoatType.generated.h"




UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	//descript what phase the game is in

	BUILDINGPHASE,    //Building phase, allow 
	BUILDINGTOCOMBAT,  //setting wave stat and other things before combat, plays combat cue
	COMBAT,            //combat phase
	POSTCOMBAT,        //gives player time to finish/kill all enemy on the scene
	UNDEFINE           //default phase -> something went wrong
};


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	TOWER,
	MONSTER,
};
