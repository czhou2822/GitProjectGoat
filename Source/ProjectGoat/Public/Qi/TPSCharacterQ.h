// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/ConstructorHelpers.h"
#include "ProjectGoat/ProjectGoatType.h"
#include "CoreMinimal.h"
#include "Character/Misc/TowerSeed.h"
#include "CollisionShape.h"
#include "GameFramework/Character.h"
#include "Chaos/Transform.h"
#include "Delegates/DelegateCombinations.h"
#include "Character/Tower/TowerBase.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "TPSCharacterQ.generated.h"

//#include "WidgetBlueprint'/Game/CC/widget/widget/UMG_Pause.UMG_Pause'"


class UInventoryComponent;
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterPlacingDelegate);


UCLASS()
class PROJECTGOAT_API ATPSCharacterQ : public ACharacter
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterAims, bool, bIsCharacterAiming);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStartPlacing, bool, bEnterPlacingMode);
	
	UPROPERTY(BlueprintAssignable, Category = "TowerPlaced")
	FOnCharacterStartPlacing OnCharacterStartPlacing;

	UPROPERTY(BlueprintAssignable, Category = "C++")
	FOnCharacterAims OnCharacterAims;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* HitSnowMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UChildActorComponent* WeaponSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
	bool bAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* FireAnima;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHalfHeight = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator AimOffsetRotator = FRotator(15, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AimOffsetTranslation = FVector(130, 70, 50);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponRange = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* fireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* OnSnowCollectSound;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C++")
	bool bAllowSnowNegative;  //allow snow to go nagetive

	UPROPERTY(EditAnywhere, Category = seed)
	TSubclassOf<class ATowerSeed> SeedClass;

	FTimerHandle SnowTimer;

	FTimerHandle TowerAdjustTimer;

	float DefaultFOV;

	FTransform DefaultCameraTransform;

	float DefaultCameraArmLength;

	bool IsSelecting;

	bool IsInShop;

	UPROPERTY(BlueprintReadwrite, Category = "C++")
	bool IsCharacterPlacingTower = false;

	// used to notify whether a constructor is being throw (i.e. a tower being build) 
	// player can only enter TowerPlacingMode when this is true(i.e. no tower is being build/all constructor is on the ground)
	UPROPERTY(BlueprintReadwrite, Category = "C++")
	bool bIsConstructorPlacedDown = true;

	bool BuildCounter = true;

	bool IsTDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	ATowerBase* SpawnedTower;

	class ABulkheadPlayerState* BulkheadPlayerState;

	class ABulkheadGameState* BulkheadGameState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* GMAudioComponent_Suck;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* GMAudioComponent_CharacterBreath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundWave* SWCharacterFootStep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundWave* SWCharacterDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundWave* SWCharacterBreath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundWave* SWCharacterJump;

	bool bMovingF;

	bool bMovingR;

	UPROPERTY(BlueprintReadWrite, Category = "Bulkhead | Frost Cannon")
	float SnowCountPercentage = 0.f;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Sets default values for this character's properties
	ATPSCharacterQ();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION(EditAnywhere)
	void OnCollectSnow(FVector location);

	void MoveForward(float v);

	void MoveRight(float v);

	void JumpFunction();

	void AimStart();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AimStartBlueprintInterface();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AdjustTowerLocationInterface(FVector TowerLocation);

	void AimEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AimEndBlueprintInterface();

	void FireStart();

	void FireEnd();

	UFUNCTION(BlueprintCallable)
	void CoinCollect(int32 InGold);

	void static CoinConsume();

	//void CollectSnow();

	//void CollectEnd();

	//void CollectStart();

	UFUNCTION(BlueprintCallable)
	void SetupVariables();

	void SelectTower();

	void SelectTowerEnd();

	void InputActionBuild();

	void InputActionCancel();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TowerBuilding")
	void BuildSuccessed(FVector BuildingLocation);

	UFUNCTION(BlueprintCallable, Category = "TowerBuilding")
	void TurnConstructorIntoTower();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TowerBuilding")
	void BuildCancelled();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TowerBuilding")
	void PulloutBuildingCamera();

	ATowerBase* WhichTower();

	UFUNCTION(BlueprintCallable, Category = "TowerBuilding")
	void AdjustTowerLocation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TowerBuilding")
	void ResetBuildingCamera();

	void FastForward();

	void FastForwardEnd();

	FHitResult GetScreentoWorldLocation();

	bool isMoving();

	int32 GetTowerID(const ETowerType& InTowerType) const;
};
