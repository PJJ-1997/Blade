// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponstructures.h"
#include "NiagaraFunctionLibrary.h"
#include "CEnemy.generated.h"

UCLASS()
class U06_BATTLE_API ACEnemy 
    : public ACharacter
    , public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
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
private:
    UFUNCTION()
        void  RestoreColor();

public:
    // AActor::TakeDamage
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
        class AController * EventInstigator, AActor * DamageCauser) override;

	void ApplyFireDamage(float DotDamage, float Duration);
	void ApplyIceEffect(float SlowDuration);
	void PlayHitEffect(UNiagaraSystem* Effect,float Duration);
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

     FTimerHandle  RestoreColor_TimerHandle;

public:
	virtual void StealthDead();

	UPROPERTY(EditAnywhere, Category = "Stelath")
		UAnimMontage* StealthDeadMontage;
};
