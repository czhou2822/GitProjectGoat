// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "FrostCannon.generated.h"

/**
 *
 */
UCLASS()
class PROJECTGOAT_API AFrostCannon : public AWeaponBase
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* FrostHitCapsule;
	





public:
	AFrostCannon();

	





};
