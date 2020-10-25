// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Misc/EnemyRouteMarker.h"
#include "EnemyRoute.generated.h"

UCLASS()
class PROJECTGOAT_API AEnemyRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* NavSplinePoints;

	UPROPERTY(BlueprintReadWrite)
	TArray<class USplineMeshComponent*> SplineMeshes;

	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> NavPoints;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TArray<AEnemyRouteMarker*> RouteMarkerArray;

	//whether this route is shown
	//e.g. it should only shown when this is an active path
	//it should also shown when in editor
	UPROPERTY(BlueprintReadWrite)
	bool bIsShown = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AEnemyRoute();

	void CreateSplineMesh();

	UFUNCTION(BlueprintCallable)
	void RouteSplineToArray();

	void SetSplineMeshesIsHidden(const bool bIsHidden);

};
