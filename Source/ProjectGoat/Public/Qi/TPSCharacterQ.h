// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Misc/TowerSeed.h"
#include "CollisionShape.h"
#include "GameFramework/Character.h"
#include "TPSCharacterQ.generated.h"


class UInventoryComponent;

UCLASS()
class PROJECTGOAT_API ATPSCharacterQ : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacterQ();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent * SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent * Camera;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class USkeletalMeshComponent* tpsGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UChildActorComponent* WeaponSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
		bool bAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
		bool bAiming_collecting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* FireAnima;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float SnowCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CapsuleHalfHeight = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CapsuleRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator AimOffsetRotator = FRotator(15, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector AimOffsetTranslation = FVector(130, 70, 50);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponRange = 700.f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		
		
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int coinCount;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* fireSound;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool bAllowSnowNegative;  //allow snow to go nagetive

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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AimStartBlueprintInterface();
	void AimEnd();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AimEndBlueprintInterface();
	//void FireStart();
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
