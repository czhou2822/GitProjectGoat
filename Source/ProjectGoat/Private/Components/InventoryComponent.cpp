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



/*
checks if the item can be bought. 
if so, check if enough money
*/
bool UInventoryComponent::Purchase(AActor* Item)
{
	UCostComponent* CostComp;
	if (Item)
	{
		CostComp = Item->FindComponentByClass<UCostComponent>();
		if (CostComp)
		{
			if (Gold - CostComp->Cost >= 0)
			{
				Gold -= CostComp->Cost;
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Not enough money"));
				return false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Actor $s has cannot be buy. "), *Item->GetName());
		}
	}
	return false;
}




