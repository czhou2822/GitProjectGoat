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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	FRotator DoomRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	class ATowerBase* PawnOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	bool bIsPlaced;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	bool bIsConstructionComplete;


public: 
	UAnimTowerBase();

	virtual void NativeBeginPlay();
};
