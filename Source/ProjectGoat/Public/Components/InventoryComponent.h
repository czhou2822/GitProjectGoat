// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTGOAT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool PurchaseInternal(float Cost);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gold = 0;

	UFUNCTION(BlueprintCallable)
	bool PurchaseItem(AActor* Item);

	UFUNCTION(BlueprintCallable)
	bool Purchase(float Cost);
		
};
