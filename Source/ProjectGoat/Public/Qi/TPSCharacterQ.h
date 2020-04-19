// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerSeed.h"
#include "GameFramework/Character.h"
#include "TPSCharacterQ.generated.h"

UCLASS()
class PROJECTGOAT_API ATPSCharacterQ : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacterQ();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32  coinCount;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent * springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent * tpsCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* tpsGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* fireAnima;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float snowCount;
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int coinCount;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* fireSound;
	UPROPERTY(EditAnywhere, Category = seed)
		TSubclassOf<class ATowerSeed> SeedClass;

	UFUNCTION(BlueprintImplementableEvent)
		void OnCollectSnow(FVector location);

	void MoveForward(float v);
	void MoveRight(float v);
	void CrouchDown();
	void CrouchUp();
	void JumpFunction();
	void AimStart();
	void AimEnd();
	void FireStart();
	void FireDown();
	void FireUp();
	void FireEnd();
	void  coinCollect();
	void static coinConsume();
	void onOverlap (AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void throwSeed();
	void collectSnow();
	void collectUp();
	void collectDown();

	FTimerHandle fireTimer;
	FTimerHandle snowTimer;
};
