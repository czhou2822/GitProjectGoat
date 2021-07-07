// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimTowerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UAnimTowerBase : public UAnimInstance
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructionComplete);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "C++")
	FOnConstructionComplete OnConstructionComplete;

	/*
	* Turret_Rotation_JNT value, step value, used to control the rotation of the tower doom
	* i.e. setting this 1 degree in roll will roll the existing bone 1 degree instead of setting this to an absolute 1 degree
	* use step value setup to incoporate the rotation mode "add to existing" in the ABP
	* the reason to choose rotation mode "add to existing" instead of "replacing existing" is that
	* when using the latter mode, the child bone underneath, i.e. Turret_Elevation (the gun barrel) change strangly too
	* see for yourself if you are curious, the setting is in ABP - statemachine - idle
	* could be something to fix in the art end, but i would rater to fix the issue right now. 
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	FRotator DoomRotation;

	/*
	* Turret_Elevation_JNT value, used to control the gun barrel of the tower
	* step size too, see comment for DoomRotation
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	FRotator BarrelRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	class ATowerBase* PawnOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	bool bIsPlaced;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	bool bIsConstructionComplete;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	bool bCanAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	float AttackAnimPlayRate;



public: 
	UAnimTowerBase();

	virtual void NativeBeginPlay();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
