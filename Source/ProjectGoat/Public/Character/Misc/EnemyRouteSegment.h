// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRouteSegment.generated.h"

UCLASS()
class AEnemyRouteSegment : public AActor
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* NavSplinePoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<class USplineMeshComponent*> SplineMeshes;



	
public:	
	// Sets default values for this actor's properties
	AEnemyRouteSegment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
