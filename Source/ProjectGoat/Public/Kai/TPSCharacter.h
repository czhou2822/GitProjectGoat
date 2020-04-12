// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

UCLASS()
class PROJECTGOAT_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* fireSound;

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
};
