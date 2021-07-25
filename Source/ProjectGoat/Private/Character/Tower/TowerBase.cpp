// Fill out your copyright notice in the Description page of Project Settings.

//user includes
#include "Character/Tower/TowerBase.h"
#include "Qi/TPSCharacterQ.h"
#include "projectgoat/Public/BulkheadGameState.h"
#include "projectgoat/Public/BulkheadPlayerState.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Core/AnimTowerBase.h"




//engine includes 
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/MeshComponent.h"
#include "components/SkeletalMeshComponent.h"
#include "Containers/Set.h"
#include "Containers/UnrealString.h"

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

	ATPSCharacterQ* TPSCharacter = Cast<ATPSCharacterQ>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	BulkheadGameState = Cast<ABulkheadGameState>(GetWorld()->GetAuthGameMode()->GameState);
	if (BulkheadGameState)
	{
		BulkheadGameState->OnTowerPlaced.AddDynamic(this, &ATowerBase::HandleOnTowerPlaced);
	}

	BulkheadPlayerState = Cast<ABulkheadPlayerState>(BulkheadGameState->PlayerArray[0]);
	
	TowerAnim = Cast< UAnimTowerBase>(GetMesh()->GetAnimInstance());

	
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//this -> SetTargetActor();
	//this->FireTimer(NextFire);
	
	
}

void ATowerBase::HandleOnTowerPlaced(ATowerBase* SpawnedTower)
{
	//UE_LOG(LogTemp, Warning, TEXT("TowerPlaced"));
	GetCharacterData().bTeam = true;

	ATPSCharacterQ* TPSCharacter = Cast<ATPSCharacterQ>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (TPSCharacter)
	{
		BulkheadGameState->OnTowerPlaced.RemoveDynamic(this, &ATowerBase::HandleOnTowerPlaced);
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
	TowerFireTickInterface();

	if (TargetEnemy && !TargetEnemy->IsPendingKill())
	{
		FirePoint = GetMesh()->GetSocketLocation(FirePointName);

		bIsAttack = true;

		TowerFire();  //broadcast blueprint fire event
		return;
	}

	bIsAttack = false;
	return;

}

//void ATowerBase::TurnFirePointToTheEnemy()
//{
//	if (TargetEnemy)
//	{
//		FVector SocketLocation = GetMesh()->GetSocketLocation("FirePoint");
//		FVector FirePointToEnemyVector = TargetEnemy->GetActorLocation() - SocketLocation;   //direction vector pointing from fire point to target enemy
//		
//		FirePointToEnemyVector.Normalize(0.01);
//		SocketLocation.Normalize(0.01);
//
//		/*take 2d cross product of the two unit vector,
//		* if the cross product > 0, Enemy is at the left side of the fire point, turn the fire point to left by substracting TurnSpeed
//		* if the cross product < 0, enemy is at the right side of the fire point ....
//		* if the cross product nearly 0, these two vectors are either overlapped or completely opposite
//		* see the gif here "https://www.mathsisfun.com/algebra/vectors-cross-product.html"
//		*/
//		float Direction = FVector2D::CrossProduct(FVector2D(FirePointToEnemyVector.X, FirePointToEnemyVector.Y), FVector2D(SocketLocation.X, SocketLocation.Y));
//		UE_LOG(LogTemp, Log, TEXT("CrossProduct: %s"), *FString::SanitizeFloat(Direction));
//		if (!TowerAnim)
//		{
//			bIsAlignWithTarget = false;
//			return;
//		}
//
//		//case overlapped: two vector is aligned already
//
//		if (FMath::IsNearlyZero(Direction, FacingTolrence * (1.e-10f)))
//		{
//			bIsAlignWithTarget = true;
//			return;
//		}
//
//		if (Direction>0)
//		{
//			TowerAnim->DoomRotation.Yaw -= TurnSpeed;
//		}
//		else
//		{
//			TowerAnim->DoomRotation.Yaw += TurnSpeed;
//		}
//
//		bIsAlignWithTarget = false;
//		return;
//	}
//}


void ATowerBase::TurnFirePointToTheEnemy()
{
	if (TargetEnemy)
	{
		FVector SocketLocation = GetMesh()->GetSocketLocation("FirePoint");
		FVector EnemyLocation = TargetEnemy->GetActorLocation();   //direction vector pointing from fire point to target enemy


		EnemyLocation;

	}
}

void ATowerBase::PitchFirePointToEnemy()
{
	if (TargetEnemy)
	{
		FVector SocketLocation = GetMesh()->GetSocketLocation("FirePoint");

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetEnemy->GetActorLocation());//the rotation this fire point eventually turns to


	}
}

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