// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/AnimTowerBase.h"
#include "Character/Tower/TowerBase.h"

UAnimTowerBase::UAnimTowerBase()
	:bIsPlaced(false)
	,bIsConstructionComplete(false)
	,PawnOwner(nullptr)
	,DoomRotation(FRotator::ZeroRotator)
{
	
}

void UAnimTowerBase::NativeBeginPlay()
{
//	bIsPlaced = false;
//	bIsConstructionComplete = false;
	PawnOwner = Cast<ATowerBase>(TryGetPawnOwner());
//	bIsPlaced;
//	bIsConstructionComplete;
}
