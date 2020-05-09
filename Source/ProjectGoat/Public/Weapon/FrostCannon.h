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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* AimBox;




public:
	AFrostCannon();

	virtual void Fire() override;
	virtual void FireEnd() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnFireStart();
	UFUNCTION(BlueprintImplementableEvent)
		void OnFireEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnAimUp();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnAimDown();

};
