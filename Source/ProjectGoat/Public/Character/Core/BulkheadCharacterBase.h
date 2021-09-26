// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BulkheadGameState.h"
#include "Data/CharacterData.h"
#include "BulkheadCharacterBase.generated.h"



UCLASS()
class PROJECTGOAT_API ABulkheadCharacterBase : public ACharacter
{
	GENERATED_BODY()


	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = "true"))
	//class UWidgetComponent* HealthWidget;


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBulkheadCharacterDead);

	UPROPERTY(BlueprintAssignable, Category = "BulkheadCharacterBase")
	FOnBulkheadCharacterDead OnBulkheadCharacterDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BulkheadCharacterBase")
	FGuid GUID;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulkheadCharacterBase")
	//class USceneComponent* HomingPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BulkheadCharacterBase")
	bool bIsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BulkheadCharacterBase")
	class USoundAttenuation* SoundAttenuationSetting;

	UFUNCTION(BlueprintImplementableEvent, Category = "BulkheadCharacterBase")
	void OnReceivedDamage(const float ReceivedDamage, const float RemainingHealthPercentage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	

	//virtual FCharacterData& GetCharacterData();

	// Sets default values for this character's properties
	ABulkheadCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BulkheadInit();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "BulkheadCharacterBase")
	virtual FCharacterData& GetCharacterData();


	virtual bool IsDead();

	virtual float GetHealth();

	virtual float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	virtual void MarkForDead();

	virtual void Dead();

	virtual void UpdateUI();

	void ResetGUID();

	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Attribute")
	bool IsActive() { return !IsDead(); }

	UFUNCTION(BlueprintCallable)
	virtual void Attack(ABulkheadCharacterBase* DamageCauser, ABulkheadCharacterBase* Target, float DamageValue);

	FORCEINLINE ABulkheadGameState* GetGameState() { return GetWorld() ? GetWorld()->GetGameState<ABulkheadGameState>() : NULL; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCharacterDying();



};
