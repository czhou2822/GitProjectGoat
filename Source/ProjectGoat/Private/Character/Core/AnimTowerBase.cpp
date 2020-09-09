// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/AnimTowerBase.h"
#include "Character/Tower/TowerBase.h"

UAnimTowerBase::UAnimTowerBase()
	:bIsPlaced(false)
	,DoomRotation(FRotator::ZeroRotator)
	,PawnOwner(nullptr)
	,bIsConstructionComplete(false)
{
	
}

void UAnimTowerBase::NativeBeginPlay()
{
	PawnOwner = Cast<ATowerBase>(TryGetPawnOwner());


}
