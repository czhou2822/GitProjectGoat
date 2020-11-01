// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "WeaponBase.generated.h"

UCLASS()
class PROJECTGOAT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
protected:
	FTimerHandle FireTimer;

	bool bShowDebugCollision = false;

public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponBaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireInvetval = 0.2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* FirePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFiring;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleOnAudioFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void FireStart();

	virtual void FireEnd();

	virtual void Fire();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		UAudioComponent* GMAudioComponent_Fire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWFireStart;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundWave* SWFireEnd;
};
