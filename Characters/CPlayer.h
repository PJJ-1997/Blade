// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Characters/CEnemy.h"
#include "Characters/CAnimInstance.h"
#include "Characters/CPlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CStatusComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Widgets/CUserWidget_Player.h"
#include "Weapons/SkillActions/CZAction_Hammer.h"
#include "Interfaces/InteractionInterface.h"
#include "CPlayer.generated.h"

class ACSTutorialHUD;
class UCWeaponComponent;
class UInventoryComponent;
class UItemBase;
class UTimelineComponent;
class UCUserWidget_Cooldown;

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

		FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
		AActor* CurrentInteractable;
	UPROPERTY()
		float LastInteractionCheckTime;
};

UCLASS()
class U06_BATTLE_API ACPlayer
	: public ACharacter
	, public IICharacter
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void  Landed(const FHitResult& Hit) override;

public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return  TeamID; }

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator, AActor * DamageCauser) override;

public:
	struct FDamageData
	{
		float  power = 0.0f;
		class  ACharacter* Character;
		class  AActor*     Causer;
		struct FActionDamageEvent* Event;
	};
	FDamageData Damage;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		FGenericTeamId TeamID = FGenericTeamId(1);

private:
	UCStatusComponent* StatusComponent;
	ACPlayerController* PlayerController;

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent*    Camera;

private:
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;
	UPROPERTY(VisibleAnywhere)
		class UCStateComponent*    State;
	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent*  Montages;
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent*    Weapon;
	UPROPERTY(VisibleAnywhere)
		class UCParkourComponent*   Parkour;
	UPROPERTY(VisibleAnywhere)
		class UCFeetComponent*      Feet;
	UPROPERTY(VisibleAnywhere)
		class UCZoomComponent*      Zoom;
public:
	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent*    Status;

private: // 파쿠르
	UPROPERTY(VisibleAnywhere)
		class USceneComponent*    ArrowGroup;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent*    Arrows[(int32)EParkourArrowType::Max];

private:
	UFUNCTION()
		void  OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
public:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType PrevType, EWeaponType NewType);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void ShowWidgets();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void HideWidgets();
private:
	void  OnAvoid();
	void  BackStep();
	void  Roll();
	void  Hitted();
	void  Dead();

private:  // 노티파이에서 호출
	virtual void End_Avoid() override;
	virtual void End_Hit()   override;
	virtual void End_Dead()  override;

private:
	void OnRightButton();
	void OffRightButton();

private:
	void OnZKeyPressed();
	void OffZKeyPressed();
	void RepeatZKeyAction();
	FTimerHandle TimerHandle;
	bool bZKeyHeld = false; // Z 키가 눌렸는지 여부를 나타냄
private:
	void OnXKeyPressed();
	void OffXKeyPressed();

private:
	void OnQKeyPressed();
	void OffQKeyPressed();

public:
	void OnCKeyPressed();
	void OffCKeyPressed();

public:
	void PerformStealthKill();
	virtual void StealthKill() override;

	UPROPERTY(EditAnywhere)
		UAnimMontage* StealthMontage;
public:
	void OnEKeyPressed();

#if WITH_EDITOR  // GameMode에서는 사용하지 않게
private:
	UPROPERTY(EditDefaultsOnly, Category = "FKey")
		class  UWidgetComponent*  FKeyWidget;
#endif

public:
	UCUserWidget_Player* HPWidget;

	void UpdateHealthAndStaminaOnWidget(float CurrentHealth, float MaxHealth, float CurrentStamina, float MaxStamina);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
		float AvoidStaminaCost = 10.0f; // 회피 동작의 스태미나 비용
private:
	void SetZooming(float InValue);

private:
	void  CreateParkourArrow();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* LandingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* BlockingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* Pickupound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* DropSound;
	UPROPERTY(EditAnywhere, Category = "Block")
		float BlockAnimationInterval;
	UPROPERTY(EditAnywhere, Category = "Block")
		float BlockAngle = 5.0f; // 왼쪽 또는 오른쪽으로 블록할 각도
private:
	void OnBlock(); // 블록 시작
	void OffBlock(); // 블록 종료
	void StartBlock();
	void StopBlock();
	void PlayBlockAnimation();

	bool bIsBlocking;

	//void OnHitReactionEnd();
	// Inventory
	bool bAiming;
public:
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };
	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };
	void UpdateInteractionWidget() const;
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);
protected:
	//=========================================================//
	// PROPERTIES & VARIABLES
	//=========================================================//
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
		TScriptInterface<IInteractionInterface> TargetInteractable;
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
		UInventoryComponent* PlayerInventory;
	UPROPERTY()
		ACSTutorialHUD* HUD;
	
	float InteractionCheckFrequency;
	float InteractionCheckDistance;
	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

	// timeline property used for camera aiming transition
	UPROPERTY(VisibleAnywhere, Category ="Character | Camera")
		FVector DefaultCameraLocation;
	UPROPERTY(VisibleAnywhere, Category = "Character | Camera")
		FVector AimingCameraLocation;

	float ProbeSize = 12.0f;
	float MaxCameraArmLength  = 800.0f;

	UTimelineComponent* AimingCameraTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Aim Timeline")
		UCurveFloat* AimingCameraCurve;


	//=========================================================//
	// FUNCTIONS
	//=========================================================//
	void ToggleMenu();
	void TogglePauseMenu();

	void Aim();
	void StopAiming();
	UFUNCTION()
	void UpdateCameraTimeline(const float TimelineValue) const;
	UFUNCTION()
	void CameraTimelineEnd();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();
	//Save & Load
public:
	void SavePlayerData();
	void LoadPlayerData();
	void SetSkillCooldown(FName SkillName, float CooldownTime);
	void UpdateCooldowns();
	bool IsSkillOnCooldown(FName SkillName) const;

private:
	TMap<FName, float> SkillCooldowns; 
	TMap<FName, FTimerHandle> CooldownTimers; 

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> CooldownWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
		UCUserWidget_Cooldown* CooldownWidget;

	FTimerHandle CooldownUpdateTimerHandle;
	
private:
	void ResetCooldown(FName SkillName);
};
