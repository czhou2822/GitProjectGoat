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
#include "EnemyBase.h"
#include "TowerSeed.h"


// Sets default values
ATPSCharacterQ::ATPSCharacterQ()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	springArm->bUsePawnControlRotation = true;
	springArm->SetRelativeLocation(FVector(0, 0, 0));
	springArm->bEnableCameraLag = true;

	tpsCamera = CreateDefaultSubobject<UCameraComponent>("tpsCamera");
	tpsCamera->SetupAttachment(springArm);


	tpsGun = CreateDefaultSubobject<USkeletalMeshComponent>("tpsGun");
	tpsGun->SetupAttachment(GetMesh(), "weapon_socket");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;
	coinCount = 0;
	//following is for collision test
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSCharacterQ::onOverlap);
	//UE_LOG(LogTemp, Warning, TEXT("start"));
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
	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, & ATPSCharacterQ::collectDown);
	PlayerInputComponent->BindAction("Collect", IE_Released, this, & ATPSCharacterQ::collectUp);
	PlayerInputComponent->BindAction("Build", IE_Pressed, this, & ATPSCharacterQ::throwSeed);
	
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
	Jump();
}

void ATPSCharacterQ::AimStart()
{
	bAiming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	springArm->SetRelativeLocation(FVector(130, 70, 50));
}

void ATPSCharacterQ::AimEnd()
{
	bAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	springArm->SetRelativeLocation(FVector(0, 0, 0));
}

void ATPSCharacterQ::FireStart()
{
	if (bAiming)
	{
		PlayAnimMontage(fireAnima);
		UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());

		FVector fireStartPoint = tpsGun->GetSocketLocation("Muzzle");
		//FVector fireEndPoint = tpsGun->GetRightVector() *5000 + fireStartPoint;
		FVector fireEndPoint = tpsCamera->GetForwardVector() * 5000 + fireStartPoint;

		DrawDebugLine(GetWorld(), fireStartPoint, fireEndPoint, FColor::Red, false, 2.f, 0, 5.f);


		FCollisionQueryParams cqp;
		FHitResult hr;

		GetWorld()->LineTraceSingleByChannel(hr, fireStartPoint, fireEndPoint, ECC_Visibility, cqp);

		if (hr.bBlockingHit == true) {
			if (hr.GetActor() != this) {

				UE_LOG(LogTemp, Warning, TEXT("HIT! %s"), *hr.GetActor()->GetName());
				UE_LOG(LogTemp, Warning, TEXT("HIT! Location: %s"), *hr.Location.ToString());
				UE_LOG(LogTemp, Warning, TEXT("HIT! ImpactPoint: %s"), *hr.ImpactPoint.ToString());
				//hr.GetActor()->Destroy();
				DrawDebugLine(GetWorld(), hr.Location, hr.Location + FVector::UpVector * 5000, FColor::Red, false, 2.f, 0, 5.f);

				if (Cast<AEnemyBase>(hr.GetActor()) != nullptr) {
					AEnemyBase* target = Cast<AEnemyBase>(hr.GetActor());
					target->SlowDown();
					UE_LOG(LogTemp, Warning, TEXT("An enemy is hit"), *hr.GetActor()->GetName());
				}
			}
		}
	}

}

void ATPSCharacterQ::FireDown() 
{
	GetWorld()->GetTimerManager().SetTimer(fireTimer,this, &ATPSCharacterQ::FireStart, 0.2f, true, 0.f);
	
}
void ATPSCharacterQ::FireUp()
{
	GetWorld()->GetTimerManager().ClearTimer(fireTimer);

}
void ATPSCharacterQ::FireEnd()
{
}

/*void ATPSCharacterQ::coinCollect()
{
	ATPSCharacterQ::coinCount++;
}*/

void ATPSCharacterQ::coinCollect() {
	coinCount++;
	UE_LOG(LogTemp, Warning,TEXT("coinCount:"),coinCount,coinCount);
	//coinCount.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(coinCount));
}

void ATPSCharacterQ::onOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ATPSCharacterQ::throwSeed()
{
	FVector seedStartPoint = tpsGun->GetSocketLocation("Muzzle");
	if (bAiming) {
		if (SeedClass)
		{
			// Get the camera transform.
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			// Transform MuzzleOffset from camera space to world space.
			FVector MuzzleLocation = seedStartPoint;// CameraLocation + FTransform(CameraRotation).TransformVector(seedStartPoint);
			FRotator MuzzleRotation = CameraRotation;
			// Skew the aim to be slightly upwards.
			MuzzleRotation.Pitch += 10.0f;
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				// Spawn the projectile at the muzzle.
				ATowerSeed* seed = World->SpawnActor<ATowerSeed>(SeedClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (seed)
				{
					// Set the projectile's initial trajectory.
					FVector LaunchDirection = MuzzleRotation.Vector();
					seed->FireInDirection(LaunchDirection);
				}
			}
		}
	}
}

void ATPSCharacterQ::collectSnow()
{
	if (bAiming)
	{
		/*PlayAnimMontage(fireAnima);
		UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());*/

		FVector fireStartPoint = tpsGun->GetSocketLocation("Muzzle");
		//FVector fireEndPoint = tpsGun->GetRightVector() *5000 + fireStartPoint;
		FVector fireEndPoint = tpsCamera->GetForwardVector() * 500 + fireStartPoint;

		DrawDebugLine(GetWorld(), fireStartPoint, fireEndPoint, FColor::Blue, false, 2.f, 0, 5.f);


		FCollisionQueryParams cqp;
		FHitResult hr;

		GetWorld()->LineTraceSingleByChannel(hr, fireStartPoint, fireEndPoint, ECollisionChannel::ECC_GameTraceChannel3, cqp);

		if (hr.bBlockingHit == true) {
			if (hr.GetActor() != this) {

				UE_LOG(LogTemp, Warning, TEXT("HIT! %s"), *hr.GetActor()->GetName());
				UE_LOG(LogTemp, Warning, TEXT("HIT! Location: %s"), *hr.Location.ToString());
				UE_LOG(LogTemp, Warning, TEXT("HIT! ImpactPoint: %s"), *hr.ImpactPoint.ToString());
				//hr.GetActor()->Destroy();
				DrawDebugLine(GetWorld(), hr.Location, hr.Location + FVector::UpVector * 5000, FColor::Blue, false, 2.f, 0, 5.f);
				if (snowCount <= 99) {
					snowCount++;
				}
				else {
					snowCount = 100;
				}
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::SanitizeFloat(snowCount));
				
			}
		}
	}
}

void ATPSCharacterQ::collectUp()
{
	GetWorld()->GetTimerManager().ClearTimer(snowTimer);
}

void ATPSCharacterQ::collectDown()
{
	GetWorld()->GetTimerManager().SetTimer(snowTimer, this, &ATPSCharacterQ::collectSnow, 0.1f, true, 0.f);
}

