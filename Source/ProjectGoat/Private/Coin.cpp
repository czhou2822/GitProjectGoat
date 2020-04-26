// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PointLightComponent.h"
#include "Qi/TPSCharacterQ.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	//sphere->SetHiddenInGame(false);
	RootComponent = VisualMesh;
	SphereComp->SetupAttachment(RootComponent);
	//VisualMesh->SetupAttachment(RootComponent);
	//tried to set the type of collision 
	VisualMesh->BodyInstance.SetCollisionProfileName(TEXT("pickups"));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACoin::HandleOnSphereBeginOverlap);
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	/*AActor* actor = GetOwner();
	USphereComponent* mesh = actor->FindComponentByClass<USphereComponent>();*/
	/*VisualMesh->OnComponentBeginOverlap.AddDynamic(this,&ACoin::collect());*/
	/*VisualMesh->OnComponentHit.AddDynamic(this,&ACoin::OnHit());
	sphere->OnComponentBeginOverlap.AddDynamic(this, &ACoin::collect());*/
	//sphere->OnComponentHit.AddDynamic(this, &ACoin::OnHit);
	//VisualMesh->OnComponentHit.AddDynamic(this, &ACoin::OnHit);
	//VisualMesh->OnComponentBeginOverlap.AddDynamic(this, &ACoin::collect);
	//GetCapsuleMesh()->OnComponentBeginOverlap.addDynamic(this, &ACoin::OnHit);
}
void ACoin::HandleOnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("!!!!!!"));
	ATPSCharacterQ* PlayerCharacter = Cast<ATPSCharacterQ>(OtherActor);
	if (PlayerCharacter)
	{
		//Hit.GetActor()->coinCollect();
		//Cast<ATPSCharacterQ>(OtherActor)->coinCollect();
		PlayerCharacter->coinCollect();
		UE_LOG(LogTemp, Warning, TEXT("now the CoinCount is: %d"), (PlayerCharacter->coinCount));
		if (PlayerCharacter->coinCount > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("everything goes fine"))
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("coin has not been correctly added"))
		}
		Destroy();

	}

	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("type cast failed"));
	}
}
void ACoin::collect(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*FScriptDelegate OverlayBegin;
	OverlayBegin.BindUFunction(this, "OnOverlayBegin");*/
	
}


