// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Characters/Boss/CBossAnimInstance.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Weapons/CWeaponstructures.h"
#include "NiagaraFunctionLibrary.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "CBoss.generated.h"

class ACActor_RangeSword;
class UNiagaraSystem;
class ACSTutorialHUD;
class UCUserWidget_Player;
class UCUserWidget_Cooldown;

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class U06_BATTLE_API ACBoss
	: public ACharacter
	, public IICharacter
{

	GENERATED_BODY()
///////////////////////////////////////////
// Boss BehaviorTree
private:
	
	FTimerHandle BlockTimerHandle;
public:
	ACBoss();
	void BackJump();
	void BasicAttack1(AActor* OtherActor);
	void BasicAttack2();
	void BasicAttack3();
	void BasicAttack4();
	void DashAttack();
	void RangeAttack();
	void DecalAttack();
	void Block();
	//void TurnLeft();
	//void TurnRight();
	void AIStart();
	void AIStop();
	void DelayedAIStart();

	FOnAttackEndDelegate OnAttackEnd;
	bool IsBlocking() const { return bIsBlocking; }
	bool bIsBlocking;
	UFUNCTION()
		void EndBlock();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* BlockSound;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UBoxComponent* HitBoxComponent;
	UFUNCTION()
		void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
/////////////////////////////////////////////
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent*   Movement;
	UPROPERTY(VisibleAnywhere)
		class UCStateComponent*      State;
	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent*    Montages;
	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent*      Status;

private:
	UPROPERTY(EditAnywhere)
		FLinearColor OriginColor = FLinearColor::Black;

private:
	UFUNCTION()
		void  OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	// AActor::TakeDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator, AActor * DamageCauser) override;

	void PlayHitEffect(UNiagaraSystem* Effect, float Duration);
	void ApplyFireDamage(float DotDamage, float Duration);
	void ApplyIceEffect(float SlowDuration);

private:
	void DealDotDamage();
	void RemoveIceEffect();

private:
	FTimerHandle DotDamageHandle;
	FTimerHandle SlowHandle;

	float TotalDotDamage;
	float RemainingDotTime;

public:   // Enmey-->EnemyAI
	virtual void Hit();
	virtual void Dead();

public:   // Interface에서 상속 받은 함수 oeveride
	virtual void End_Hit()   override;
	virtual void End_Dead()  override;


public:
	struct FDamageData
	{
		float  power = 0.0f;
		class  ACharacter* Character;
		class  AActor*     Causer;
		struct FActionDamageEvent* Event;
	};
	FDamageData Damage;

	FTimerHandle Timer;
	FTimerHandle ArrowDamageTimer;
	FTimerHandle RestartAITimerHandle;
	UPROPERTY(EditAnywhere, Category = "Components")
		FHitData HitData;

private:
	UFUNCTION()
		void PlayStartMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = true))
		UAnimMontage* StartMontage;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		bool IsAttacking = false;

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
protected:
	UPROPERTY(Transient)
		class UCBossAnimInstance* BossAnimInstance;
	UPROPERTY()
		class ACAIController_Boss* BossAI;
// About Boss Cinematic
private:
	void AddAura();
	void PlayTransformCinematic();
	void SetWidgetsVisibility(bool bVisible);

	void HideAttachedActors();
	void ShowAttachedActors();
	void HidePlayerMesh();
	void ShowPlayerMesh();

	UFUNCTION()
		void OnTransformCinematicFinished();

	UCUserWidget_Player* HPWidget;
	UCUserWidget_Cooldown* CooldownWidget;
	ACBoss_AI* BossAICharacter;
private:
	bool bBossRageMode;
	bool bHasAura; 
	UNiagaraComponent* Aura;

	UPROPERTY(EditAnywhere, Category = "2Page BodyAura")
		UNiagaraSystem* AuraNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
		ULevelSequence* BossTransformSequence;
public:
	bool IsInBossRageMode() const;
	bool bIsAlive;

public:
	UPROPERTY(EditAnywhere, Category = "Effects")
		UNiagaraSystem* FireHitEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UNiagaraSystem* IceHitEffect;

	// 함수 선언 추가
	void SetAttackIndex(int32 InIndex);
	int32 GetAttackIndex() const; // 추가
private:
	int32 AttackIndex; // 현재 공격 인덱스
};
