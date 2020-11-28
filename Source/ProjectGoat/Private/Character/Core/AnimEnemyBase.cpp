// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/AnimEnemyBase.h"
#include "Character/Enemy/EnemyBase.h"

UAnimEnemyBase::UAnimEnemyBase()
	:GlobalAnimPlayRate(1.f)
	,bIsAttacking(false)
	,ControlledPawn(nullptr)
	
{
}

void UAnimEnemyBase::NativeBeginPlay()
{
	ControlledPawn = Cast<AEnemyBase>(TryGetPawnOwner());
}

void UAnimEnemyBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(ControlledPawn)
	{
		Speed = ControlledPawn->GetVelocity().Size();
	}
}



