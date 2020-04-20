// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::SlowDown()
{
	UE_LOG(LogTemp, Warning, TEXT("SlowDown"));

	if (TickCount == 0)    //last count has finished, restart timer
	{
		TickCount = SlowedTime / TimerTickInterval;
		if (SlowCount < MaxSlowCount) {
			SlowCount++;
			float SlowPercentage = 1;
			for (int i = 0; i <= SlowCount; i++) {
				SlowPercentage = SlowDownPercentage * SlowPercentage;
			}
			OnSlowStart();
			GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * SlowPercentage;
		}
		else {
			SlowCount = MaxSlowCount + 1;
			OnFrozenStart();
		}
		GetWorldTimerManager().SetTimer(SlowTimer, this, &AEnemyBase::HandleSlowDown, TimerTickInterval, true, 0.0f);
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else
	{
		TickCount = SlowedTime / TimerTickInterval;
	}
}

void AEnemyBase::HandleSlowDown()
{

	if (TickCount <= 0)   //timer done, reset walk speed
	{

		SlowCount = 0;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

		GetWorldTimerManager().ClearTimer(SlowTimer);
		UE_LOG(LogTemp, Warning, TEXT("Back Normal"));
		OnSlowEnd();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("tick"));
		TickCount--;
	}
}
