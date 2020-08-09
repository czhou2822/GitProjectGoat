// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRoute.generated.h"

UCLASS()
class PROJECTGOAT_API AEnemyRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* NavSplinePoints;

	TArray<class USplineMeshComponent*> SplineMeshes;

	TArray<FVector> NavPoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AEnemyRoute();

	void CreateSplineMesh();

	void RouteSplineToArray();


};
