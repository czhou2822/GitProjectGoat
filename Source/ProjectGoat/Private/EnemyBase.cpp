// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * SlowDownPercentage;
		GetWorldTimerManager().SetTimer(SlowTimer, this, &AEnemyBase::HandleSlowDown, TimerTickInterval, true, 0.0f);
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
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

		GetWorldTimerManager().ClearTimer(SlowTimer);
		UE_LOG(LogTemp, Warning, TEXT("Back Normal"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("tick"));
		TickCount--;
	}
}



