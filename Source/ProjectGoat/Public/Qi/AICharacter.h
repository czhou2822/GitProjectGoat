// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class PROJECTGOAT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
<<<<<<< HEAD:Source/ProjectGoat/Public/Qi/AICharacter.h
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
=======
	ATPSCharacter();
>>>>>>> ffb5cae873d99ec23e39d972159bef63a6c45e99:Source/ProjectGoat/Public/Kai/TPSCharacter.h
	// Called every frame
	void CrouchDown();
	void CrouchUp();
	void JumpFunction();
	void AimStart();
	void AimEnd();
	void Moveforward(float v);
	/*void FireStart();
	void FireDown();
	void FireUp();
	void FireEnd();*/
	//void TakeDamage();

	void onDead();

	void onSlow();

	void slowGoes();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* tpsGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* fireAnima;
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int coinCount;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float speedPercentage=1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int slowCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float slowDuration = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* fireSound;
<<<<<<< HEAD:Source/ProjectGoat/Public/Qi/AICharacter.h
	FTimerHandle slowTimer;
=======

	FTimerHandle fireTimerHandle;

	void MoveForward(float v);
	void MoveRight(float v);
	void CrouchDown();
	void CrouchUp();
	void JumpFunction();
	void AimStart();
	void AimEnd();
	void FireStart();
	void FireEnd();
	void Fire();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitLandScape(AActor *actorPtr,FVector location );

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
>>>>>>> ffb5cae873d99ec23e39d972159bef63a6c45e99:Source/ProjectGoat/Public/Kai/TPSCharacter.h
};
