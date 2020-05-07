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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BulkCount = 0;

private:
	bool PurchaseInternal(float Cost);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	bool PurchaseItem(AActor* Item);

	UFUNCTION(BlueprintCallable)
	bool Purchase(float Cost);
		
};
