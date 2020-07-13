// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

// Sets default values
AEnemyBase::AEnemyBase()
	:ABulkheadCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	//GetCapsuleComponent()->OnComponentBeginOverlap->AddDynamic(this, &AEnemyBase::onOverlap);

	//NavPoints.Init();
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}
/*void AEnemyBase::onOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	this->SlowDown();
}*/
// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




void AEnemyBase::SlowDown()
{
	//UE_LOG(LogTemp, Warning, TEXT("SlowDown"));


	TickCount = SlowedTime / TimerTickInterval;
	float SlowPercentage = 1;
	if (SlowCount < MaxSlowCount)
	{
		SlowCount++;

		for (int i = 0; i <= SlowCount; i++)
		{
			SlowPercentage = SlowDownPercentage * SlowPercentage;
		}
		OnSlowStart();
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * SlowPercentage;
	}
	else
	{
		SlowCount = MaxSlowCount;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		OnFrozenStart();
	}

	GetWorldTimerManager().SetTimer(SlowTimer, this, &AEnemyBase::HandleSlowDown, TimerTickInterval, true, 0.0f);

}

void AEnemyBase::StartSlow()
{
	GetWorld()->GetTimerManager().SetTimer(SlowListener, this, &AEnemyBase::SlowDown, 0.2f, true, 0.f);

}

void AEnemyBase::EndSlow()
{
	GetWorld()->GetTimerManager().ClearTimer(SlowListener);

}

//void AEnemyBase::SetNavPoints(TArray<FVector> InPoints)
//{
//	NavPoints = InPoints;
//	NavPoints.Num();
//
//}

void AEnemyBase::HandleSlowDown()
{

	if (TickCount <= 0)   //timer done, reset walk speed
	{

		SlowCount = 0;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

		GetWorldTimerManager().ClearTimer(SlowTimer);
		//UE_LOG(LogTemp, Log, TEXT("Back Normal"));
		OnSlowEnd();
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("slow down tick"));
		TickCount--;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::SanitizeFloat(GetCharacterMovement()->MaxWalkSpeed));
}
