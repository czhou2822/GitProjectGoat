// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UAnimEnemyBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	class AEnemyBase* ControlledPawn;

public:
	UAnimEnemyBase();

	virtual void NativeBeginPlay();
	
	virtual void NativeUpdateAnimation(float DeltaSeconds);
};
