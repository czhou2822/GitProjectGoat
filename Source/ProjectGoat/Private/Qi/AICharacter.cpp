// Fill out your copyright notice in the Description page of Project Settings.


#include "Qi/AICharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	tpsGun = CreateDefaultSubobject<USkeletalMeshComponent>("tpsGun");
	tpsGun->SetupAttachment(GetMesh(), "weapon_socket");
	//Moveforward(0.2f);

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void AAICharacter::TakeDamage()
{

}*/

void AAICharacter::onDead()
{

}

void AAICharacter::onSlow()
{
	if (slowCount < 4) {
		slowCount++;
	}
	else {
		slowCount = 5;
	}
	slowDuration = 3; 
	GetWorld()->GetTimerManager().ClearTimer(slowTimer);
	GetWorld()->GetTimerManager().SetTimer(slowTimer, this, &AAICharacter::slowGoes, 1.f, true, 0.f);
}
void AAICharacter::slowGoes() {
	--slowDuration;
	speedPercentage = 1;
	if (slowDuration <= 0) {
		slowCount = 0;
		GetWorld()->GetTimerManager().ClearTimer(slowTimer);
	}
	for (int i = 0; i < slowCount; i++) {
		speedPercentage = speedPercentage * 0.8f;
	}
		
}
void AAICharacter::CrouchDown()
{
	Crouch();

}

void AAICharacter::CrouchUp()
{
	UnCrouch();
}

void AAICharacter::JumpFunction()
{
	Jump();
}

void AAICharacter::AimStart()
{
	bAiming = true;
	/*GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	//springArm->SetRelativeLocation(FVector(130, 70, 50));*/
}

void AAICharacter::AimEnd()
{
	bAiming = false;
	/*GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	//springArm->SetRelativeLocation(FVector(0, 0, 0));*/
}

void AAICharacter::Moveforward(float v)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetActorRotation()),v);
	
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	Moveforward(speedPercentage);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,FString::SanitizeFloat(speedPercentage));
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

