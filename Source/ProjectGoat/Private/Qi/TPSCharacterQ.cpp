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
#include "Qi/AICharacter.h"
#include "Character/Enemy/EnemyBase.h"
#include "TowerSeed.h"
#include "CollisionShape.h"
#include "Engine/EngineTypes.h"
#include "Math/Quat.h"
#include "Components/InventoryComponent.h"
#include "Weapon/WeaponBase.h"


// Sets default values
ATPSCharacterQ::ATPSCharacterQ()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("tpsCamera");
	Camera->SetupAttachment(SpringArm);

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("InventoryComp");

	//tpsGun = CreateDefaultSubobject<USkeletalMeshComponent>("tpsGun");
	//tpsGun->SetupAttachment(GetMesh(), "weapon_socket");

	WeaponSlot = CreateDefaultSubobject<UChildActorComponent>("WeaponSlot");
	WeaponSlot->SetupAttachment(GetMesh(), "weapon_socket");
	//WeaponSlot->SetChildActorClass(AWeaponBase::StaticClass());
	//WeaponSlot->CreateChildActor();


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;

	//following is for collision test
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSCharacterQ::onOverlap);
	//UE_LOG(LogTemp, Warning, TEXT("start"));
	//tpsCamera->
}

// Called when the game starts or when spawned
void ATPSCharacterQ::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATPSCharacterQ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
/*void ATPSCharacterQ::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacterQ::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacterQ::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacterQ::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacterQ::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacterQ::CrouchDown);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacterQ::CrouchUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSCharacterQ::JumpFunction);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATPSCharacterQ::AimStart);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATPSCharacterQ::AimEnd);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacterQ::FireDown);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSCharacterQ::FireUp);
	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, & ATPSCharacterQ::collectDown);
	PlayerInputComponent->BindAction("Collect", IE_Released, this, & ATPSCharacterQ::collectUp);
	PlayerInputComponent->BindAction("Build", IE_Pressed, this, & ATPSCharacterQ::throwSeed);

}*/

void ATPSCharacterQ::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacterQ::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacterQ::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacterQ::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacterQ::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacterQ::CrouchDown);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacterQ::CrouchUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSCharacterQ::JumpFunction);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATPSCharacterQ::AimStart);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATPSCharacterQ::AimEnd);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacterQ::FireDown);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSCharacterQ::FireUp);
	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &ATPSCharacterQ::collectDown);
	PlayerInputComponent->BindAction("Collect", IE_Released, this, &ATPSCharacterQ::collectUp);
	PlayerInputComponent->BindAction("Build", IE_Pressed, this, &ATPSCharacterQ::FireEnd);

}

void ATPSCharacterQ::MoveForward(float v)
{

	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), v);
}

void ATPSCharacterQ::MoveRight(float v)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), v);
}

void ATPSCharacterQ::CrouchDown()
{
	Crouch();

}

void ATPSCharacterQ::CrouchUp()
{
	UnCrouch();
}

void ATPSCharacterQ::JumpFunction()
{

	//UE_LOG(LogTemp, Warning, TEXT("Jump"));
	Jump();
}

void ATPSCharacterQ::AimStart()
{
	bAiming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	SpringArm->SetRelativeLocation(AimOffsetTranslation);
	Camera->SetRelativeRotation(AimOffsetRotator);
	AimStartBlueprintInterface();
}

void ATPSCharacterQ::AimEnd()
{
	bAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	Camera->SetRelativeRotation(FRotator(0,0,0));
	AimEndBlueprintInterface();
}

//void ATPSCharacterQ::FireStart()
//{
//
//
//	if (bAiming)
//	{
//		if (!bAllowSnowNegative)   //not allow snow to go under 0, play mode
//		{
//			if (SnowCount <= 0)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Frost Cannon empty! Aim and Press F to collect snow"));
//				return;
//			}
//		}
//		SnowCount -= 0.5;
//
//		PlayAnimMontage(FireAnima);
//		
//
//
//		//FVector fireStartPoint = tpsGun->GetSocketLocation("Muzzle");
//
//		if(AWeaponBase* Weapon = Cast<AWeaponBase>( WeaponSlot->GetChildActor()))
//		{
//			FVector fireStartPoint = Weapon->FirePoint->GetComponentLocation();
//
//
//
//			int32 ScreenX;
//			int32 ScreenY;
//			GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);
//
//			//UE_LOG(LogTemp, Warning, TEXT("Screen size %d, %d"), ScreenX, ScreenY);
//
//			FVector WorldLocation;
//			FVector WorldDirection;
//			GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(ScreenX / 2, ScreenY / 2, WorldLocation, WorldDirection);
//			//FVector fireEndPoint = WorldDirection * WeaponRange + WorldLocation;
//
//
//			//fireEndPoint = tpsCamera->GetForwardVector() * WeaponRange + fireStartPoint;
//			FVector fireEndPoint = Camera->GetForwardVector() * WeaponRange + GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
//			//fireEndPoint = tpsCamera->GetForwardVector() * WeaponRange + tpsCamera->GetComponentLocation();
//
//
//
//
//			//FVector fireEndPoint = tpsCamera->GetForwardVector() * 5000 + fireStartPoint;
//			FVector SweepStart = fireStartPoint;
//			//FVector SweepEnd = tpsCamera->GetForwardVector() * 600 + SweepStart;  //backup
//			FVector SweepEnd = fireEndPoint;
//
//			//DrawDebugLine(GetWorld(), fireStartPoint, fireEndPoint, FColor::Red, false, 2.f, 0, 5.f);
//
//			FCollisionShape MyColShape = FCollisionShape::MakeCapsule(CapsuleRadius, (WeaponRange - 2 * CapsuleRadius) / 2);
//			FVector eyeLocation;
//			FRotator eyeRotation;
//			GetActorEyesViewPoint(eyeLocation, eyeRotation);
//			//manual offset
//			/*eyeRotation.Pitch += 115;
//			eyeRotation.Yaw += 3;*/
//			FRotationConversionCache WorldRotationCache;
//			FQuat ShapeQuat = WorldRotationCache.RotatorToQuat(eyeRotation);
//			//DrawDebugCapsule(GetWorld(), tpsCamera->GetForwardVector() * (MyColShape.GetCapsuleHalfHeight()-150 )+ fireStartPoint, MyColShape.GetCapsuleHalfHeight()-150, MyColShape.GetCapsuleRadius(), ShapeQuat, FColor::White, false, 0.5f);
//
//			DrawDebugCapsule(GetWorld(), (fireStartPoint + fireEndPoint) / 2, MyColShape.GetCapsuleHalfHeight(), MyColShape.GetCapsuleRadius(), ShapeQuat, FColor::Red, false, 1.f);
//
//
//			FCollisionQueryParams cqp;
//			FHitResult hr;
//			TArray<FHitResult> hrShape;
//			//DrawDebugBox(GetWorld(), tpsCamera->GetForwardVector() * 250 + fireStartPoint, FVector(100, 20, 50), FColor::Purple, true);
//			//GetWorld()->LineTraceSingleByChannel(hr, fireStartPoint, fireEndPoint, ECC_GameTraceChannel7, cqp);
//			bool isHit = GetWorld()->SweepMultiByChannel(hrShape, SweepStart, SweepEnd, ShapeQuat, ECC_GameTraceChannel7, MyColShape);
//			SnowCount--;
//
//			if (isHit)
//			{
//				UE_LOG(LogTemp, Warning, TEXT("hitted actors: %d"), hrShape.Num());
//				for (int i = 0; i < hrShape.Num(); i++)
//				{
//					hr = hrShape[i];
//					if (hr.GetActor() && hr.GetActor() != this)
//					{
//						//UE_LOG(LogTemp, Warning, TEXT("HIT! %s"), *hr.GetActor()->GetName());
//						//UE_LOG(LogTemp, Warning, TEXT("HIT! Location: %s"), *hr.Location.ToString());
//						//UE_LOG(LogTemp, Warning, TEXT("HIT! ImpactPoint: %s"), *hr.ImpactPoint.ToString());
//						//hr.GetActor()->Destroy();
//						//DrawDebugLine(GetWorld(), hr.Location, hr.Location + FVector::UpVector * 5000, FColor::Red, false, 2.f, 0, 5.f);
//
//						if (Cast<AEnemyBase>(hr.GetActor()) != nullptr)
//						{
//							AEnemyBase* target = Cast<AEnemyBase>(hr.GetActor());
//							target->SlowDown();
//							UE_LOG(LogTemp, Warning, TEXT("An enemy is hit"), *hr.GetActor()->GetName());
//						}
//					}
//				}
//			}
//		}
//
//
//
//
//	}
//}

void ATPSCharacterQ::FireDown()
{
	//GetWorld()->GetTimerManager().SetTimer(fireTimer, this, &ATPSCharacterQ::FireStart, 0.2f, true, 0.f);

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
void ATPSCharacterQ::FireUp()
{
	//GetWorld()->GetTimerManager().ClearTimer(fireTimer);
	AWeaponBase* WeaponDummy = Cast<AWeaponBase>(WeaponSlot->GetChildActor());
	if (WeaponDummy)
	{
		//UE_LOG(LogTemp, Log, TEXT("Weapon valid"));
		WeaponDummy->FireEnd();
	}
}
void ATPSCharacterQ::FireEnd()
{
}

/*void ATPSCharacterQ::coinCollect()
{
	ATPSCharacterQ::coinCount++;
}*/

void ATPSCharacterQ::coinCollect()
{

	InventoryComp->Gold++;

}

void ATPSCharacterQ::onOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATPSCharacterQ::throwSeed()
{
	//FVector seedStartPoint = tpsGun->GetSocketLocation("Muzzle");
	//if (bAiming) {
	//	if (SeedClass)
	//	{
	//		// Get the camera transform.
	//		FVector CameraLocation;
	//		FRotator CameraRotation;
	//		GetActorEyesViewPoint(CameraLocation, CameraRotation);

	//		// Transform MuzzleOffset from camera space to world space.
	//		FVector MuzzleLocation = seedStartPoint;// CameraLocation + FTransform(CameraRotation).TransformVector(seedStartPoint);
	//		FRotator MuzzleRotation = CameraRotation;
	//		// Skew the aim to be slightly upwards.
	//		MuzzleRotation.Pitch += 10.0f;
	//		UWorld* World = GetWorld();
	//		if (World)
	//		{
	//			FActorSpawnParameters SpawnParams;
	//			SpawnParams.Owner = this;
	//			SpawnParams.Instigator = Instigator;
	//			// Spawn the projectile at the muzzle.
	//			ATowerSeed* seed = World->SpawnActor<ATowerSeed>(SeedClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	//			if (seed)
	//			{
	//				// Set the projectile's initial trajectory.
	//				FVector LaunchDirection = MuzzleRotation.Vector();
	//				seed->FireInDirection(LaunchDirection);
	//			}
	//		}
	//	}
	//}
}

void ATPSCharacterQ::collectSnow()
{
	if (bAiming)
	{
		/*PlayAnimMontage(fireAnima);
		UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());*/

		//FVector fireStartPoint = tpsGun->GetSocketLocation("Muzzle");
		FVector fireStartPoint = Camera->GetComponentLocation();

		//FVector fireStartPoint =  Scene->GetLocation
		//FVector fireEndPoint = tpsGun->GetRightVector() *5000 + fireStartPoint;
		FVector fireEndPoint = Camera->GetForwardVector() * 1000 + Camera->GetComponentLocation();



		//DrawDebugLine(GetWorld(), fireStartPoint, fireEndPoint, FColor::Blue, false, 2.f, 0, 5.f);


		FCollisionQueryParams cqp;
		FHitResult hr;



		GetWorld()->LineTraceSingleByChannel(hr, fireStartPoint, fireEndPoint, ECollisionChannel::ECC_GameTraceChannel3, cqp);


		if (hr.bBlockingHit == true) {
			if (hr.GetActor() != this) {

				UE_LOG(LogTemp, Warning, TEXT("HIT! %s"), *hr.GetActor()->GetName());
				UE_LOG(LogTemp, Warning, TEXT("HIT! Location: %s"), *hr.Location.ToString());
				UE_LOG(LogTemp, Warning, TEXT("HIT! ImpactPoint: %s"), *hr.ImpactPoint.ToString());
				//hr.GetActor()->Destroy();
				bAiming_collecting = true;
				OnCollectSnow(hr.Location);
				//DrawDebugLine(GetWorld(), hr.Location, hr.Location + FVector::UpVector * 5000, FColor::Blue, false, 2.f, 0, 5.f);
				if (SnowCount <= 99) {
					SnowCount++;
				}
				else {
					SnowCount = 100;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::SanitizeFloat(snowCount));

			}
		}

	}
}

void ATPSCharacterQ::collectUp()
{
	GetWorld()->GetTimerManager().ClearTimer(snowTimer);
	bAiming_collecting = false;

}

void ATPSCharacterQ::collectDown()
{
	GetWorld()->GetTimerManager().SetTimer(snowTimer, this, &ATPSCharacterQ::collectSnow, 0.1f, true, 0.f);
}

