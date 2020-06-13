// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_BulkheadUIBase.h"
#include "UI_Health.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGOAT_API UUI_Health : public UUI_BulkheadUIBase
{
	GENERATED_BODY()


	//UPROPERTY(meta = (BindWidget))
	//class UTextBlock* Title;
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Health;


public:
	virtual void NativeConstruct() override;

	void SetTitle(const FString& Msg);

	void SetHealth(float HeatlhValue);
	
};
