// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/AnimTowerBase.h"
#include "Character/Tower/TowerBase.h"

UAnimTowerBase::UAnimTowerBase()
	:DoomRotation(FRotator::ZeroRotator)
	,PawnOwner(nullptr)
	,bIsPlaced(false)
	,bIsConstructionComplete(false)
	,bIsAttack(false)
{
	
}

void UAnimTowerBase::NativeBeginPlay()
{
	PawnOwner = Cast<ATowerBase>(TryGetPawnOwner());

}

void UAnimTowerBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (PawnOwner)
	{
		bIsAttack = PawnOwner->bIsAttack;

	}
}
