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
#include "components/SkeletalMeshComponent.h"

//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif

// Sets default values
ATowerBase::ATowerBase():ABulkheadCharacterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterData().bTeam = true;
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(RootComponent);
	//Mesher = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesher"));
	//Mesher->SetupAttachment(RootComponent);
	TowerPadding = CreateDefaultSubobject<UBoxComponent>(TEXT("TowerPadding"));
	TowerPadding->SetupAttachment(RootComponent);
	OverlappedTower.Empty();

	Decal->SetVisibility(true);
	
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
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//this -> SetTargetActor();
	this->FireTimer(NextFire);
}

void ATowerBase::HandleOnTowerPlaced()
{
	UE_LOG(LogTemp, Warning, TEXT("TowerPlaced"));
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
}
void ATowerBase::SetRangeVisibility(bool InVisibility)
{
	//UDecalComponent* Decal;
	Decal->SetVisibility(InVisibility, false);
}
void ATowerBase::HandleOnCharacterStartPlacing(bool PlacingMode)
{
	this->SetRangeVisibility(PlacingMode);
}


void ATowerBase::HandleOnConstructionComplete()
{
	this->TowerInit();
	
	this->TowerFire.AddDynamic(this, &ATowerBase::HandleFireEvent);
	
	FirePoint = GetMesh()->GetSocketLocation(FirePointName);
}
void ATowerBase::TowerInit()
{
	TowerDamage = GetCharacterData().Attack;
	FireInterval = GetCharacterData().AttackRate;
}
void ATowerBase::HandleFireEvent() 
{

}
void ATowerBase::OnConstructionCompleteEvent()
{

}
void ATowerBase::SetTargetActor()
{
	
}
void ATowerBase::FireTimer(float B)
{
	float GameTime;
	GameTime = GetGameTimeSinceCreation();
	//AEnemyBase* TargetActor = Cast<AEnemyBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if ( GameTime > B)
	{
		FirePoint = GetMesh()->GetSocketLocation(FirePointName);
		this->TowerFire.Broadcast();
		NextFire = GameTime + FireInterval;
	}
}
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
}


//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif