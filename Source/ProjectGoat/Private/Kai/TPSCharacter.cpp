// Fill out your copyright notice in the Description page of Project Settings.


#include "Kai/TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
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
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacter::CrouchDown);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacter::CrouchUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSCharacter::JumpFunction);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATPSCharacter::AimStart);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATPSCharacter::AimEnd);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacter::FireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSCharacter::FireEnd);
}

void ATPSCharacter::MoveForward(float v)
{

	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), v);
}

void ATPSCharacter::MoveRight(float v)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), v);
}

void ATPSCharacter::CrouchDown()
{
	Crouch();

}

void ATPSCharacter::CrouchUp()
{
	UnCrouch();
}

void ATPSCharacter::JumpFunction()
{
	Jump();
}

void ATPSCharacter::AimStart()
{
	bAiming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	springArm->SetRelativeLocation(FVector(130, 70, 50));
}

void ATPSCharacter::AimEnd()
{
	bAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	springArm->SetRelativeLocation(FVector(0, 0, 0));
}

void ATPSCharacter::FireStart()
{
	Fire();
	GetWorld()->GetTimerManager().SetTimer(fireTimerHandle, this, &ATPSCharacter::Fire, 0.1, true);
}

void ATPSCharacter::FireEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(fireTimerHandle);
}

void ATPSCharacter::Fire()
{
	if (bAiming)
	{
		PlayAnimMontage(fireAnima);
		UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());

		FVector fireStartPoint = tpsGun->GetSocketLocation("Muzzle");
		//FVector fireEndPoint = tpsGun->GetRightVector() *5000 + fireStartPoint;
		FVector fireEndPoint = tpsCamera->GetForwardVector() * 5000 + fireStartPoint;

		DrawDebugLine(GetWorld(), fireStartPoint, fireEndPoint, FColor::Red, false, 1.f, 0, 5.f);


		FCollisionQueryParams cqp;
		FHitResult hr;

		GetWorld()->LineTraceSingleByChannel(hr, fireStartPoint, fireEndPoint, ECC_Visibility, cqp);

		if (hr.bBlockingHit == true) {
			if (hr.GetActor() != this) {

				UE_LOG(LogTemp, Warning, TEXT("HIT! %s"), *hr.GetActor()->GetName());
				UE_LOG(LogTemp, Warning, TEXT("HIT! Location: %s"), *hr.Location.ToString());
				UE_LOG(LogTemp, Warning, TEXT("HIT! ImpactPoint: %s"), *hr.ImpactPoint.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screen Message"));
				//hr.GetActor()->Destroy();
				//DrawDebugLine(GetWorld(), hr.Location, hr.Location + FVector::UpVector * 100, FColor::Red, false, 2.f, 0, 5.f);
				OnHitLandScape(hr.GetActor(), hr.Location);
			}
		}
	}
}




