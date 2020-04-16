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
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
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
	FTimerHandle slowTimer;
};
