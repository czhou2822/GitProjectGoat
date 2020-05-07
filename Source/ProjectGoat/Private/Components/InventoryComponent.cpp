// Fill out your copyright notice in the Description page of Project Settings.



#include "Components/InventoryComponent.h"
#include "Components/CostComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

bool UInventoryComponent::PurchaseInternal(float Cost)
{
	if (Gold - Cost >= 0)
	{
		Gold -= Cost;
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not enough money"));
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Not enough money"));
		return false;
	}
}


bool UInventoryComponent::Purchase(float Cost)
{
	return PurchaseInternal(Cost);
}



/*
checks if the item can be bought. 
if so, check if enough money
*/
bool UInventoryComponent::PurchaseItem(AActor* Item)
{
	UCostComponent* CostComp;
	if (Item)
	{
		CostComp = Item->FindComponentByClass<UCostComponent>();
		if (CostComp)
		{
			return PurchaseInternal(CostComp->Cost);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Actor $s has cannot be buy. "), *Item->GetName());
		}
	}
	return false;
}




