// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Core/BulkheadCharacterBase.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Character/UI_Health.h"
#include "EngineUtils.h"

//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif

// Sets default values
ABulkheadCharacterBase::ABulkheadCharacterBase()
	:bIsAttack(false)
	, SoundAttenuationSetting(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	//HealthWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//HomingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPoint"));
	//HomingPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

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
	FCharacterData& DataTemp = GetCharacterData();
	
	ABulkheadCharacterBase* Attacker = Cast<ABulkheadCharacterBase>(DamageCauser);

	if (Attacker && DataTemp.ID != -1)
	{
		if (DataTemp.bTeam != Attacker->GetCharacterData().bTeam)
		{
			Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

			GetCharacterData().Health -= Damage;

			UE_LOG(LogTemp, Warning, TEXT("%s taking damage %s, remaing health %s / %s"), *GetName(), *FString::SanitizeFloat(Damage), *FString::SanitizeFloat(GetCharacterData().Health), *FString::SanitizeFloat(GetCharacterData().MaxHealth));

			if (!IsActive())   //id = -1 -> CharacterNULL
			{
				GetCharacterData().Health = 0.0f;
				OnCharacterDying();
			}

			UpdateUI();
			OnReceivedDamage(Damage, DataTemp.Health/DataTemp.MaxHealth);
		}

	}

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

void ABulkheadCharacterBase::BulkheadInit()
{
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

void ABulkheadCharacterBase::MarkForDead()
{
	GetGameState()->RemoveCharacterData(GUID);
	OnBulkheadCharacterDead.Broadcast();
	Dead();
}

void ABulkheadCharacterBase::Dead()
{
	Destroy();
}

void ABulkheadCharacterBase::Attack(ABulkheadCharacterBase* DamageCauser, ABulkheadCharacterBase* Target, float DamageValue)
{
	if (Target)
	{
		UGameplayStatics::ApplyDamage(
			Target,
			DamageValue,
			DamageCauser->GetController(),
			DamageCauser,
			UDamageType::StaticClass());
	}

}

void ABulkheadCharacterBase::OnCharacterDying_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("C++TestFunction"));
	MarkForDead();
}

void ABulkheadCharacterBase::UpdateUI()
{
	//if (HealthWidget)
	//{
	//	if (UUI_Health* HealthUI = Cast<UUI_Health>(HealthWidget->GetUserWidgetObject()))
	//	{
	//		HealthUI->SetTitle(GetCharacterData().Name.ToString());
	//		HealthUI->SetHealth(GetHealth() / GetMaxHealth());
	//	}
	//}

}

void ABulkheadCharacterBase::ResetGUID()
{
	GUID = FGuid::NewGuid();
}



//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif