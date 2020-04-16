// Fill out your copyright notice in the Description page of Project Settings.


#include "Kai/AAACharacter.h"

// Sets default values
AAAACharacter::AAAACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAAACharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAAACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAAACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

