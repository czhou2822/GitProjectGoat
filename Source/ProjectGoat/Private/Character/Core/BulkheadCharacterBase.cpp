// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Character/Core/BulkheadCharacterBase.h"

// Sets default values
ABulkheadCharacterBase::ABulkheadCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GUID = FGuid::NewGuid();
}

// Called when the game starts or when spawned
void ABulkheadCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GetController())
	{
		SpawnDefaultController();
	}
	
}

float ABulkheadCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	GetCharacterData().Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("%d"), GetCharacterData().Health)

	if (!IsActive())
	{
		GetCharacterData().Health = 0.0f;
		Dying();
	}

	//if (DrawTextClass)
	//{
	//	if (ADrawText* MyValueText = GetWorld()->SpawnActor<ADrawText>(DrawTextClass, GetActorLocation(), FRotator::ZeroRotator))
	//	{
	//		FString DamageText = FString::Printf(TEXT("-%0.f"), DamageValue);
	//		MyValueText->SetTextBlock(DamageText, FLinearColor::Red, DamageValue / GetCharacterData().MaxHealth);

	//	}
	//}

	//UpdateUI();

	return Damage;
}

// Called every frame
void ABulkheadCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABulkheadCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FCharacterData& ABulkheadCharacterBase::GetCharacterData()
{
	if (GetGameState())
	{
		return GetGameState()->GetCharacterData(GUID);
	}
	return CharacterDataNULL;
}

bool ABulkheadCharacterBase::IsDead()
{
	return GetHealth() <= 0.f;
}

float ABulkheadCharacterBase::GetHealth()
{
	return GetCharacterData().Health;
}

float ABulkheadCharacterBase::GetMaxHealth()
{
	return GetCharacterData().MaxHealth;
}

void ABulkheadCharacterBase::Dying()
{
	GetGameState()->RemoveCharacterData(GUID);
	Dead();
}

void ABulkheadCharacterBase::Dead()
{
	Destroy();
}

void ABulkheadCharacterBase::Attack(ABulkheadCharacterBase* DamageCauser, AActor* Target, float DamageValue)
{
	UGameplayStatics::ApplyDamage(
		Target,
		DamageValue,
		DamageCauser->GetController(),
		DamageCauser,
		UDamageType::StaticClass());
}

