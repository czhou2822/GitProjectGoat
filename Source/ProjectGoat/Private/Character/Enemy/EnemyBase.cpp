// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectGoat/ProjectGoatGameMode.h"
#include "Character/Core/AnimEnemyBase.h"


// Sets default values
AEnemyBase::AEnemyBase()
	:ABulkheadCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	//GetCapsuleComponent()->OnComponentBeginOverlap->AddDynamic(this, &AEnemyBase::onOverlap);

	//NavPoints.Init();
	USkeletalMeshComponent* mesh = GetMesh();
	mesh->SetScalarParameterValueOnMaterials("SnowAmount", 0);
	
	GMAudioComponent_EnemyBreath = CreateDefaultSubobject<UAudioComponent>(TEXT("EnemyBreath"));


}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	//gets the needed counter to frozen the enemy. TimerTickInterval is set to 0.1. 
	//e.g. if FrozenTime = 2.f and TimerTickInterval = 0.1; MaxFrozenMeter = 2/0.1 = 20;
	//while frozen, a timer increase FrozenMeter by 1 every 0.1 seconds. 
	MaxFrozenMeter = FrozenTime / FrozenTimerTickInterval;

	//similar to last line. 
	MaxFrozenPausedMeter = FrozenPauseTime / FrozenTimerTickInterval;
}

AEnemyBase::~AEnemyBase()
{

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::BulkheadInit()
{
	Super::BulkheadInit();
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterData().RunSpeed;
	GetCharacterData().bTeam = false;

	if (SWEnemyBreath)
	{
		GMAudioComponent_EnemyBreath->Sound = SWEnemyBreath;
		GMAudioComponent_EnemyBreath->Play();
	}
	
}



/*
* whenever the frost cannon fires, frost cannon loop through its overlapping enemy list and call this function 
* also, the frost cannon firing is also controlled by a timer -- the timer starts when the fire button is pressed and ends when the fire button is released
* therefore, if the firing timer has calls every 0.2 seconds, this function is also called every 0.2 seconds
*/
void AEnemyBase::FreezeStart()
{
	//whenver this function is called, this enemy is either maintaining frozen status or just starts to frozen up. Either way,
	//we need to reset the FrozenPausedMeter and mark for being freezing
	FrozenPausedMeter = MaxFrozenPausedMeter;
	bIsFreezing = true;

	//if the timer is running already, no need to reenable the timer 
	if (FrozenTimer.IsValid())
	{
		return;
	}

	GetWorldTimerManager().SetTimer(FrozenTimer, this, &AEnemyBase::FrozenTimerTick, FrozenTimerTickInterval, true, 0.0f);

	//if (FrozenMeter < MaxFrozenMeter)
	//{
	//	FrozenMeter++;

	//	for (int i = 0; i <= FrozenMeter; i++)
	//	{
	//		SlowPercentage = SlowDownPercentage * SlowPercentage;
	//	}
	//	OnSlowStart();
	//}
	//else
	//{
	//	FrozenMeter = MaxFrozenMeter;
	//	SlowPercentage = 0;
	//	OnFrozenStart();
	//}

	//SlowPercentage in action
	//GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * SlowPercentage;

	//SetAnimGlobalPlayrate(SlowPercentage);


}

void AEnemyBase::FrozenTimerTick()
{
	//percentage. 0->no frozen at all, 1-> completly frozen 
	float FrozenProgress = 0;

	//if is being frozed, count up
	if (bIsFreezing)
	{
		/*
		* so this is how it works: 
		* FreezeStart() always set bIsFreezing to true so we need to reset it here so that if FreezeStart() is not being call/enemy is not being freezing
		* we can use this variable to check
		*/
		FrozenMeter++;
		//clamping frozen meter to the MaxFrozenMeter
		FrozenMeter = FMath::Min(FrozenMeter, MaxFrozenMeter);
		bIsFreezing = false;
	}

	//if is not being frozed, start counting pause counter
	else
	{
		//frozen paused logic
		if (FrozenPausedMeter >= 0)  //still in frozen paused phase
		{
			FrozenPausedMeter--;
		}
		else if (FrozenPausedMeter < 0)
		{
			FrozenMeter--;
		}
	}

	if (FrozenMeter <= 0)//frozen ends, reset everything
	{
		GetWorld()->GetTimerManager().ClearTimer(FrozenTimer);
	}

	FrozenProgress = (float)FrozenMeter / (float)MaxFrozenMeter;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * (1.f - FrozenProgress);
	SetAnimGlobalPlayrate(1.f - FrozenProgress);
	OnFrozen(FrozenProgress);


	//if (TickCount <= 0)   //timer done, reset walk speed
	//{
	//	FrozenMeter = 0;
	//	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

	//	GetWorldTimerManager().ClearTimer(FrozenTimer);
	//	SetAnimGlobalPlayrate(1.f);

	//	OnSlowEnd();
	//}
	//else
	//{
	//	TickCount--;
	//}
}

void AEnemyBase::StartSlow()
{
	GetWorld()->GetTimerManager().SetTimer(SlowListener, this, &AEnemyBase::FreezeStart, 0.2f, true, 0.f);

}

void AEnemyBase::EndSlow()
{
	GetWorld()->GetTimerManager().ClearTimer(SlowListener);
}

void AEnemyBase::StartBrittle()
{
	AProjectGoatGameMode* gameMode = Cast<AProjectGoatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->SetIsBrittle(GUID, true);
	if (SWEnemyBreath)
	{
		//GMAudioComponent_EnemyBreath->Sound = SWEnemyBreath;
		GMAudioComponent_EnemyBreath->Stop();
	}
}

void AEnemyBase::EndBrittle()
{
	AProjectGoatGameMode* gameMode = Cast<AProjectGoatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->SetIsBrittle(GUID, false);
	if (SWEnemyBreath)
	{
		//GMAudioComponent_EnemyBreath->Sound = SWEnemyBreath;
		GMAudioComponent_EnemyBreath->Play();
	}
}
void AEnemyBase::SetAnimGlobalPlayrate(float InPlayRate)
{
	//slow animation for slowing effect
	auto EnemyAnim = Cast<UAnimEnemyBase>(GetMesh()->GetAnimInstance());

	if (EnemyAnim)
	{
		EnemyAnim->GlobalAnimPlayRate = InPlayRate;
	}
}


float AEnemyBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//APawn::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//ABulkheadGameState* InGameState = Cast<ABulkheadGameState>(GetGameState());


	float NewDamage = Damage;

	if (GetCharacterData().bIsBrittle == true)
	{
		NewDamage = NewDamage * BrittleDamageRate;
	}

	ABulkheadCharacterBase::TakeDamage(NewDamage, DamageEvent, EventInstigator, DamageCauser);


	return NewDamage;
}

void AEnemyBase::MarkForDead()
{
	if (SWEnemyDeath)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SWEnemyDeath);
	}

	GetGameState()->DeleteFromPrioritizedList(this);
	GetGameState()->DeleteActiveEnemy(this);
	Super::MarkForDead();


}
