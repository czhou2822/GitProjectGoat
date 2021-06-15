// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Tower/TowerBase.h"
#include "MortarTower.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API AMortarTower : public ATowerBase
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mortar", meta = (ClampMin = "0", ClampMax = "5"))
	float AirTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mortar")
	float FiringVelocity = 3000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mortar", meta = (ClampMin = "45", ClampMax = "80"))
	int FiringAngleMin = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mortar", meta = (ClampMin = "45", ClampMax = "80"))
	int FiringAngleMax = 80;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mortar")
	float TravelDistanceMin = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mortar")
	float TravelDistanceMax = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mortar")
	float VerticalTravelDistanceMin = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mortar")
	float VerticalTravelDistanceMax = 0.f;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;
#endif

public:

	AMortarTower();

	UFUNCTION(BlueprintPure, Category = "Mortar")
	float GetAngle(FVector InPosition);




	
};
