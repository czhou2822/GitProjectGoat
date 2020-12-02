// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Tower/TowerBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Qi/TPSCharacterQ.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Containers/Set.h"
#include "Containers/UnrealString.h"
#include "projectgoat/Public/BulkheadGameState.h"
#include "projectgoat/Public/BulkheadPlayerState.h"
#include "Components/MeshComponent.h"
#include "components/SkeletalMeshComponent.h"
#include "Character/Enemy/EnemyBase.h"

#include "Kismet/GameplayStatics.h"

//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif

// Sets default values
ATowerBase::ATowerBase():ABulkheadCharacterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerPadding = CreateDefaultSubobject<UBoxComponent>(TEXT("TowerPadding"));
	TowerPadding->SetupAttachment(RootComponent);
	OverlappedTower.Empty();

	RangeMeshC = CreateDefaultSubobject<UStaticMeshComponent >(TEXT("RangeMeshC"));
	RangeMeshC->SetupAttachment(RootComponent);

	RangeMeshC->SetVisibility(false);

	//TowerInit();
	
}



// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	ATPSCharacterQ* TPSCharacter = Cast<ATPSCharacterQ>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (TPSCharacter)
	{
		TPSCharacter->OnTowerPlaced.AddDynamic(this, &ATowerBase::HandleOnTowerPlaced);
	}
	BulkheadGameState = Cast<ABulkheadGameState>(GetWorld()->GetAuthGameMode()->GameState);
	BulkheadPlayerState = Cast<ABulkheadPlayerState>(BulkheadGameState->PlayerArray[0]);
	
	
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//this -> SetTargetActor();
	//this->FireTimer(NextFire);
	
	
}

void ATowerBase::HandleOnTowerPlaced()
{
	//UE_LOG(LogTemp, Warning, TEXT("TowerPlaced"));
	GetCharacterData().bTeam = true;

	ATPSCharacterQ* TPSCharacter = Cast<ATPSCharacterQ>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (TPSCharacter)
	{
		TPSCharacter->OnTowerPlaced.RemoveDynamic(this, &ATowerBase::HandleOnTowerPlaced);
		TPSCharacter->OnCharacterStartPlacing.AddDynamic(this, &ATowerBase::HandleOnCharacterStartPlacing);
	}
	this->SetRangeVisibility(false);
	if (OverlappedTower.Num())   //overlapped tower exist, delete tower
	{
		this->Destroy();
		for (auto It = OverlappedTower.CreateConstIterator(); It; ++It)
		{	
			ATowerBase* Tower = Cast<ATowerBase>(*It);
			FString AMessage;
			FString BMessage;
			FString Message;
			AMessage = "overlap with";
			BMessage = UKismetSystemLibrary::GetDisplayName(Tower);
			Message = AMessage.Append(BMessage);
			UKismetSystemLibrary::PrintString(this->GetWorld(), Message, true, true, FLinearColor(1, 0, 0, 0.5), 2.0);
			return;
		}
	}
	InternalTowerPlaced.Broadcast();
	//if (BulkheadPlayerState)
	//{
	//	switch (BulkheadPlayerState->SelectedTower)
	//	{
	//	case ETowerType::TESLA:
	//		UGameplayStatics::PlaySound2D(this, SWTeslaTowerConstruction);
	//		break;
	//	case ETowerType::MORTAR:
	//		UGameplayStatics::PlaySound2D(this, SWMortarTowerConstruction);
	//		break;
	//	case ETowerType::GATLING:
	//		UGameplayStatics::PlaySound2D(this, SWGatlingTowerConstruction);
	//		break;
	//	}

	//}
	
}

void ATowerBase::SetRangeVisibility(bool InVisibility)
{
//	//UDecalComponent* Decal;
	RangeMeshC->SetVisibility(InVisibility, false);
	UE_LOG(LogTemp, Warning, TEXT("Visibility Set to %i"), InVisibility);
}

void ATowerBase::HandleOnCharacterStartPlacing(bool PlacingMode)
{
	this->SetRangeVisibility(PlacingMode);
	UE_LOG(LogTemp, Warning, TEXT("handle on character start placing by %s"), *GetName());

}

void ATowerBase::HandleOnConstructionComplete()
{
	
	
	//this->TowerFire.AddDynamic(this, &ATowerBase::HandleFireEvent);
	
	TowerInit();

}

void ATowerBase::TowerInit()
{
	FCharacterData ThisData = GetCharacterData();
	TowerDamage = ThisData.Attack;
	FireInterval = ThisData.AttackRate;
	SelfTowerID = ThisData.ID;
	TowerRange = ThisData.Range;


	//GetWorld()->GetTimerManager().SetTimer(FireTimerHandler, this, &ATowerBase::FireTimerTick, FireInterval, true);
	GetWorldTimerManager().SetTimer(FireTimerHandler, this, &ATowerBase::FireTimerTick, FireInterval, true, 0.0f);

}

void ATowerBase::FireTimerTick()
{
	if (TargetEnemy && !TargetEnemy->IsPendingKill())
	{
		FirePoint = GetMesh()->GetSocketLocation(FirePointName);

		bIsAttack = true;

		//TowerFire();  //broadcast blueprint fire event
		return;
	}

	bIsAttack = false;
	return;

}

//void ATowerBase::HandleFireEvent() 
//{
//	//Attack
//	
//}

void ATowerBase::BulkheadInit()
{
	FCharacterData ThisData = GetCharacterData();
	TowerDamage = ThisData.Attack;
	FireInterval = ThisData.AttackRate;
	SelfTowerID = ThisData.ID;
	TowerRange = ThisData.Range;
}

void ATowerBase::PlayFireSound()
{
	if (BulkheadPlayerState)
	{
		if (SWTowerAttack)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SWTowerAttack, FirePoint, 1.f, 1.f, 0.f, SoundAttenuationSetting, nullptr);
		}

	}
}

float& ATowerBase::GetTowerRange() 
{
	return GetCharacterData().Range;
}



void ATowerBase::OnConstructionCompleteEvent()
{

}

void ATowerBase::SetTargetActor()
{
	
}

//void ATowerBase::FireTimer(float B)
//{
//	float GameTime;
//	GameTime = GetGameTimeSinceCreation();
//	//AEnemyBase* TargetActor = Cast<AEnemyBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
//	
//	if ( GameTime > B)
//	{
//		FirePoint = GetMesh()->GetSocketLocation(FirePointName);
//		this->TowerFire.Broadcast();
//		NextFire = GameTime + FireInterval;
//	}
//}

void ATowerBase::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString AMessage;
	FString BMessage;
	FString Message;
	if (IsPlaced == false)
	{
		ATowerBase* TowerBase = Cast<ATowerBase>(OtherActor);
		this->SetRedOutLine(true);
		AMessage = UKismetSystemLibrary::GetDisplayName(TowerBase);
		BMessage = "overlapped!";
		Message = AMessage.Append(BMessage);
		UKismetSystemLibrary::PrintString(this->GetWorld(), Message, true, true, FLinearColor(1, 0, 0, 0.5), 2.0);
		OverlappedTower.Add(TowerBase);
	}
}

void ATowerBase::SetRedOutLine(bool IsOutline)
{
	float ParameterValue;
	if (IsOutline == true)
	{
		ParameterValue = 1;
	}
	else
	{
		ParameterValue = 0;
	}
	//this->SetScalarParameterValue("isOutLine", ParameterValue);
}

void ATowerBase::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FString AMessage;
	FString BMessage;
	FString Message;
	ATowerBase* TowerBase = Cast<ATowerBase>(OtherActor);
	SetRedOutLine(false);
	OverlappedTower.Remove(TowerBase);
	AMessage = UKismetSystemLibrary::GetDisplayName(TowerBase);
	BMessage = "removed!";
	Message = AMessage.Append(BMessage);
	UKismetSystemLibrary::PrintString(this->GetWorld(), Message, true, true, FLinearColor(1, 0, 0, 0.5), 2.0);
	UGameplayStatics::PlaySound2D(this, SWTowerDestruction);
}


//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif