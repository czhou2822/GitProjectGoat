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

}

AEnemyBase::~AEnemyBase()
{
	if (GetWorld())
	{
		MarkForDead();

	}
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




void AEnemyBase::SlowDown()
{


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
	}
	else
	{
		SlowCount = MaxSlowCount;
		SlowPercentage = 0;
		OnFrozenStart();

	}

	//SlowPercentage in action
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * SlowPercentage;

	SetAnimGlobalPlayrate(SlowPercentage);

	GetWorldTimerManager().SetTimer(SlowTimer, this, &AEnemyBase::HandleSlowDown, TimerTickInterval, true, 0.0f);

}

void AEnemyBase::HandleSlowDown()
{

	if (TickCount <= 0)   //timer done, reset walk speed
	{

		SlowCount = 0;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

		GetWorldTimerManager().ClearTimer(SlowTimer);
		SetAnimGlobalPlayrate(1.f);

		OnSlowEnd();
	}
	else
	{
		TickCount--;
	}
}

void AEnemyBase::StartSlow()
{
	GetWorld()->GetTimerManager().SetTimer(SlowListener, this, &AEnemyBase::SlowDown, 0.2f, true, 0.f);

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
