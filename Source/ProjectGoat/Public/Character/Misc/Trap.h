// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class PROJECTGOAT_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite, Category = "CPP")
	int32 BaseHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CPP")
	int32 BaseHPDefault = 50;

	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* NaviPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UArrowComponent* FacingArrow;

public:	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CPP")
	void UpdateHealthUI();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetBaseNavPoint();

	

};
