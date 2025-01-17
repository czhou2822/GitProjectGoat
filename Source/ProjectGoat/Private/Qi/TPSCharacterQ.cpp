// Fill out your copyright notice in the Description page of Project Settings.


#include "Qi/TPSCharacterQ.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "Windows/LiveCoding/Private/External/LC_StringUtil.h"
#include "Logging/LogMacros.h"
#include "GameFramework/Actor.h"
#include "Character/Enemy/EnemyBase.h"
#include "CollisionShape.h"
#include "Engine/EngineTypes.h"
#include "Math/Quat.h"
#include "Components/InventoryComponent.h"
#include "Weapon/WeaponBase.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialInterface.h"
#include "Character/Tower/TowerBase.h"
#include "../ProjectGoatGameMode.h"
#include "Components/CapsuleComponent.h"
#include "projectgoat/Public/BulkheadGameState.h"

//#if PLATFORM_WINDOWS
//#pragma optimize("", on)
//#endif


// Sets default values
ATPSCharacterQ::ATPSCharacterQ()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	SpringArm->bEnableCameraLag = true;

	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("tpsCamera");
	Camera->SetupAttachment(SpringArm);



	WeaponSlot = CreateDefaultSubobject<UChildActorComponent>("WeaponSlot");
	WeaponSlot->SetupAttachment(GetMesh(), "weapon_socket");



	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;

	GMAudioComponent_Suck = CreateDefaultSubobject<UAudioComponent>(TEXT("CharacterSuck"));
	
}

// Called when the game starts or when spawned
void ATPSCharacterQ::BeginPlay()
{
	Super::BeginPlay();

	BulkheadGameState = Cast<ABulkheadGameState>(GetWorld()->GetAuthGameMode()->GameState);

	BulkheadPlayerState = Cast<ABulkheadPlayerState>(BulkheadGameState->PlayerArray[0]);

}

// Called every frame
void ATPSCharacterQ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATPSCharacterQ::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacterQ::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacterQ::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacterQ::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacterQ::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSCharacterQ::JumpFunction);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATPSCharacterQ::AimStart);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATPSCharacterQ::AimEnd);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacterQ::FireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSCharacterQ::FireEnd);

	//PlayerInputComponent->BindAction("Collector", IE_Pressed, this, &ATPSCharacterQ::CollectStart);
	//PlayerInputComponent->BindAction("Collector", IE_Released, this, &ATPSCharacterQ::CollectEnd);

	PlayerInputComponent->BindAction("Build", IE_Pressed, this, &ATPSCharacterQ::InputActionBuild);

	PlayerInputComponent->BindAction("SelectTower", IE_Pressed, this, &ATPSCharacterQ::SelectTower);
	PlayerInputComponent->BindAction("SelectTower", IE_Released, this, &ATPSCharacterQ::SelectTowerEnd);

	PlayerInputComponent->BindAction("Cancel", IE_Pressed, this, &ATPSCharacterQ::InputActionCancel);

	//PlayerInputComponent->BindAction("OpenMenu", IE_Pressed, this, &ATPSCharacterQ::OpenMenu);

	PlayerInputComponent->BindAction("FastForward", IE_Pressed, this, &ATPSCharacterQ::FastForward);
	PlayerInputComponent->BindAction("FastForward", IE_Released, this, &ATPSCharacterQ::FastForwardEnd);

}

void ATPSCharacterQ::MoveForward(float v)
{

	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), v);
	if (v != 0) {
		bMovingF = true;
		UGameplayStatics::PlaySound2D(GetWorld(),SWCharacterBreath);
	}
	else 
	{
		bMovingF = false;
	}

}

void ATPSCharacterQ::MoveRight(float v)
{

	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), v);
	if (v != 0) {
		bMovingR = true;
		UGameplayStatics::PlaySound2D(GetWorld(), SWCharacterBreath);
	}
	else
	{
		bMovingR = false;
	}
}

void ATPSCharacterQ::JumpFunction()
{
	Jump();
	if (SWCharacterJump) {
		UGameplayStatics::PlaySound2D(GetWorld(), SWCharacterJump);
	}

}

void ATPSCharacterQ::AimStart()
{
	bAiming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	// SpringArm->SetRelativeLocation(AimOffsetTranslation);
	Camera->SetRelativeRotation(AimOffsetRotator);
	AimStartBlueprintInterface();
	OnCharacterAims.Broadcast(bAiming);
}

void ATPSCharacterQ::AimEnd()
{
	bAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	// SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	Camera->SetRelativeRotation(FRotator(0, 0, 0));
	AimEndBlueprintInterface();
	OnCharacterAims.Broadcast(bAiming);

}

void ATPSCharacterQ::FireStart()
{

	AWeaponBase* WeaponDummy = Cast<AWeaponBase>(WeaponSlot->GetChildActor());
	if (WeaponDummy)
	{
		if (bAiming)
		{
			WeaponDummy->FireStart();
		}
		//UE_LOG(LogTemp, Log, TEXT("Weapon valid"));
	}

}

void ATPSCharacterQ::FireEnd()
{
	AWeaponBase* WeaponDummy = Cast<AWeaponBase>(WeaponSlot->GetChildActor());
	if (WeaponDummy)
	{
		if (bAiming)
		{
			WeaponDummy->FireEnd();
		}
	}
}

//void ATPSCharacterQ::CoinCollect(int32 InGold)
//{
//	//InventoryComp->Gold += InGold;
//}

//void ATPSCharacterQ::CollectEnd()
//{
//	GetWorld()->GetTimerManager().ClearTimer(SnowTimer);
//	bAiming_collecting = false;
//
//}
//
//void ATPSCharacterQ::CollectStart()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "CollectDown");
//	GetWorld()->GetTimerManager().SetTimer(SnowTimer, this, &ATPSCharacterQ::CollectSnow, SnowChargingTickInterval, true, 0.f);
//
//}



void ATPSCharacterQ::SetupVariables()
{
	BulkheadPlayerState->SetTowerType(ETowerType::MORTAR);
	DefaultFOV = Camera->FieldOfView;
	DefaultCameraTransform = Camera->GetRelativeTransform();
	DefaultCameraArmLength = SpringArm->TargetArmLength;
}

void ATPSCharacterQ::SelectTower()
{
	IsSelecting = true;
	if (BulkheadPlayerState)
	{
		BulkheadPlayerState->NextTower();
	}
}

void ATPSCharacterQ::SelectTowerEnd()
{
	IsSelecting = false;

}

void ATPSCharacterQ::InputActionBuild()
{
	if (BuildCounter)
	{
		SpawnedTower = WhichTower();
		if (SpawnedTower)
		{
			PulloutBuildingCamera();
			IsCharacterPlacingTower = true;
			this->OnCharacterStartPlacing.Broadcast(true);
			GetWorld()->GetTimerManager().SetTimer(TowerAdjustTimer, this, &ATPSCharacterQ::AdjustTowerLocation, 0.016667f, true, 0.f);
			BuildCounter = false;
		}
	}
	else
	{
		if (IsCharacterPlacingTower == true)
		{
			//tower preview is valid
			if (SpawnedTower)
			{
				IsCharacterPlacingTower = false;
				//from here, the player has thrown the constructor out -> constructor is in the air -> IsConstructorPlacedDown = false
				bIsConstructorPlacedDown = false;

				if (BulkheadGameState && BulkheadPlayerState)
				{
					int32 Cost = 0;
					int32 TowerID = GetTowerID(BulkheadPlayerState->SelectedTower);
					Cost = BulkheadGameState->GetCharacterDataByID(TowerID)->Gold;

					if (SpawnedTower->bCanBeBuilt)
					{
						if(BulkheadPlayerState->ConsumeCoin(Cost))
						{
							//success, build tower
							//which tower are we trying to build?
							ETowerType TowerType = BulkheadPlayerState->SelectedTower;

							BuildSuccessed(SpawnedTower, TowerType);

							//this->OnTowerPlaced.Broadcast();
						}
						else
						{
							//if the build action is invalid, reset hologram tower & reset flags
							BuildCancelled();
							SpawnedTower->Destroy();
							SpawnedTower = nullptr;
							bIsConstructorPlacedDown = true;
							BulkheadPlayerState->OnBuildFailed.Broadcast();
						}
					}
					else
					{
						//if the build action is invalid, reset hologram tower & reset flags
						BuildCancelled();
						SpawnedTower->Destroy();
						SpawnedTower = nullptr;
						bIsConstructorPlacedDown = true;
						BulkheadPlayerState->OnBuildFailed.Broadcast();
					}
				}
				GetWorld()->GetTimerManager().ClearTimer(TowerAdjustTimer);
				ResetBuildingCamera();
				this->OnCharacterStartPlacing.Broadcast(false);
				BuildCounter = true;
			}
		}
	}
}

/*
* when constructor hits the floor, call this function
* this function destroy uses preview's transform to spawn a new tower, and destroy the preview tower
*/
void ATPSCharacterQ::TurnConstructorIntoTower(ATowerBase* HoloTower, ETowerType TowerType)
{
	FTransform NewTransform = HoloTower->GetActorTransform();
	FCharacterData newData;
	HoloTower->Destroy();
	HoloTower = nullptr;

	ATowerBase *TempTower = Cast<AProjectGoatGameMode>(GetWorld()->GetAuthGameMode())->SpawnTower(GetTowerID(TowerType), newData, NewTransform.GetLocation(), NewTransform.GetRotation().Rotator());

	if (TempTower) //if false->problem spawning
	{
		//new tower has been built, reset the flag
		bIsConstructorPlacedDown = true;
		BulkheadGameState->OnTowerPlaced.Broadcast(TempTower);
	}
}

ATowerBase* ATPSCharacterQ::WhichTower()
{
	FHitResult Result = this->GetScreentoWorldLocation();
	FTransform Transform = FTransform(Result.Location);
	FRotator NewRotator = UKismetMathLibrary::FindLookAtRotation(Transform.GetLocation(), GetActorLocation());
	NewRotator.Roll = 0;
	NewRotator.Pitch = 0;

	FCharacterData newData;

	return Cast<AProjectGoatGameMode>(GetWorld()->GetAuthGameMode())->SpawnTower(3, newData, Transform.GetLocation(), NewRotator);  //3->preview tower
}

void ATPSCharacterQ::InputActionCancel()
{
	BuildCancelled();
	if (SpawnedTower) 
	{
		SpawnedTower->Destroy();
	}
	

	if (IsCharacterPlacingTower)
	{
		IsCharacterPlacingTower = false;
		//this->OnTowerPlaced.Broadcast();
		this->OnCharacterStartPlacing.Broadcast(false);
		GetWorld()->GetTimerManager().ClearTimer(TowerAdjustTimer);
		ResetBuildingCamera();
		BuildCounter = true;
	}


}


void ATPSCharacterQ::AdjustTowerLocation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "Timer activated");
	if (IsValid(SpawnedTower)) 
	{
		
		float Radius = 0;
		float HalfHeight = 0;
		HalfHeight = SpawnedTower->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		Radius = SpawnedTower->GetCapsuleComponent()->GetScaledCapsuleRadius();

		APlayerController* PlayerController;
		PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

		FVector2D result = FVector2D(1, 1);
		GEngine->GameViewport->GetViewportSize(result);
		FVector2D BuildingScreenPosition = FVector2D(0.5, 0.35);

		FVector WorldPosition;
		FVector WorldDirection;
		PlayerController->DeprojectScreenPositionToWorld(result.X * BuildingScreenPosition.X, result.Y * BuildingScreenPosition.Y, WorldPosition, WorldDirection);

		FVector StartPoint;
		FVector EndPoint;
		StartPoint = WorldPosition;
		EndPoint = WorldDirection * 4000 + WorldPosition;

		TArray<TEnumAsByte<EObjectTypeQuery>> CapsuleTraceObjectsArray;
		CapsuleTraceObjectsArray.Add(EObjectTypeQuery::ObjectTypeQuery1);
		CapsuleTraceObjectsArray.Add(EObjectTypeQuery::ObjectTypeQuery10);

		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(PlayerController->GetPawn());

		FHitResult hr;
		bool hit;
		hit = UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), StartPoint, EndPoint, Radius, HalfHeight, CapsuleTraceObjectsArray, false, IgnoreActors, EDrawDebugTrace::None, hr, true);
		float DistanceFromActor = (hr.Location - WorldPosition).Size();
		if (DistanceFromActor > 500 && hit == true)
		{
			FRotator NewRotator = UKismetMathLibrary::FindLookAtRotation(SpawnedTower->GetActorLocation(), GetActorLocation());


			NewRotator.Roll = 0;
			NewRotator.Pitch = 0;
			SpawnedTower->SetActorLocation(hr.Location, false, nullptr, ETeleportType::TeleportPhysics);
			SpawnedTower->SetActorRotation(NewRotator);
			AdjustTowerLocationInterface(hr.Location);

		}
	}
}

void ATPSCharacterQ::FastForward()
{
	IsTDown = true;
}

void ATPSCharacterQ::FastForwardEnd()
{
	IsTDown = false;
}

FHitResult ATPSCharacterQ::GetScreentoWorldLocation()
{
	APlayerController* PlayerController;
	PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	FVector2D result = FVector2D(1, 1);
	GEngine->GameViewport->GetViewportSize(result);
	result /= 2;
	FVector WorldPosition;
	FVector WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(result.X, result.Y, WorldPosition, WorldDirection);
	FVector StartPoint = WorldPosition;
	FVector EndPoint = WorldDirection * 5000 + WorldPosition;
	FHitResult hr;
	//TArray nothing[];
	TArray<TEnumAsByte<EObjectTypeQuery>> TowerTraceObjectArray;
	TowerTraceObjectArray.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TowerTraceObjectArray.Add(EObjectTypeQuery::ObjectTypeQuery10);
	TArray<AActor*, FDefaultAllocator> IgnoreActors;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartPoint, EndPoint, TowerTraceObjectArray, false, IgnoreActors, EDrawDebugTrace::None, hr, true);
	return hr;
}

bool ATPSCharacterQ::isMoving()
{
	if (bMovingF || bMovingR)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

int32 ATPSCharacterQ::GetTowerID(const ETowerType& InTowerType) const
{
	int32 SpawnTowerID = 0;
	if (BulkheadPlayerState)
	{
		switch (InTowerType)
		{
		case ETowerType::TESLA:
			break;
		case ETowerType::MORTAR:
			SpawnTowerID = 1;
			break;
		case ETowerType::GATLING:
			SpawnTowerID = 2;
		}

	}

	return SpawnTowerID;
}

//#if PLATFORM_WINDOWS
//#pragma optimize("", off)
//#endif