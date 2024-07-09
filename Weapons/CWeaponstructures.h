// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponstructures.generated.h"

// BP에서 사용된 Structure를 직렬화 여기에 구성
// 장비장착에 관련 데이터 구조체
// 구조체를 UE-C에서 구현할때는 전부 EditAnywhere

USTRUCT()
struct FEquipmentData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        class UAnimMontage* Montage;
    UPROPERTY(EditAnywhere)
        float PlayRate = 1.0f;
    UPROPERTY(EditAnywhere)
        bool  bCanMove = true;
    UPROPERTY(EditAnywhere)
        bool  bUseControlRotation = true;

};
// DoAction에 관련된 데이터
USTRUCT()
struct FDoActionData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        class UAnimMontage* Montage;
    UPROPERTY(EditAnywhere)
        float PlayRate = 1.0f;
    UPROPERTY(EditAnywhere)
        bool  bCanMove = true;
    UPROPERTY(EditAnywhere)
        bool  bFixedCamera;
    // 파티클 , 나이아가라
    UPROPERTY(EditAnywhere)
        class UFXSystemAsset* Effect; 
    UPROPERTY(EditAnywhere)
        FVector EffectLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere)
        FVector EffectScale   = FVector::OneVector;
public:
    void  DoAction(class ACharacter* InOwner);
};
// Hit에 관련된 데이터
USTRUCT()
struct FHitData
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Enemy")
        class UAnimMontage* Montage;
	
	UPROPERTY(EditAnywhere, Category = "Enemy")
        float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy")
        float Power = 0.0f;         // Damage
	UPROPERTY(EditAnywhere, Category = "Enemy")
        float Launch = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy")
        float StopTime = 0.0f;    // Hit Stop
	UPROPERTY(EditAnywhere, Category = "Enemy")
        class USoundWave* Sound;

	UPROPERTY(EditAnywhere, Category = "Enemy")
        class UFXSystemAsset* Effect;
	UPROPERTY(EditAnywhere, Category = "Enemy")
        FVector EffectLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Enemy")
        FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Boss")
		class UAnimMontage* BMontage;
	UPROPERTY(EditAnywhere, Category = "Boss")
		float BPlayRate = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Boss")
		float BPower = 0.0f;         // Damage
	UPROPERTY(EditAnywhere, Category = "Boss")
		float BLaunch = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Boss")
		float BStopTime = 0.0f;    // Hit Stop
	UPROPERTY(EditAnywhere, Category = "Boss")
		class USoundWave* BSound;

	UPROPERTY(EditAnywhere, Category = "Boss")
		class UFXSystemAsset* BEffect;
	UPROPERTY(EditAnywhere, Category = "Boss")
		FVector BEffectLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Boss")
		FVector BEffectScale = FVector::OneVector;
public:
    void SendDamage(class ACharacter*  InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
    void PlayMontage(class ACharacter* InOwner);
    void PlayHitStop(UWorld* InWorld);
    void PlaySoundWave(class ACharacter* InOwner);
    void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

	void PlayBossMontage(class ACharacter* InOwner);
	void PlayBossHitStop(UWorld* InWorld);
	void PlayBossSoundWave(class ACharacter* InOwner);
	void PlayBossEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
public:
    TArray<APawn*>  StopPawns;

};
USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
    GENERATED_BODY()
public:
   FHitData* HitData;
};

// USTRUCT()
// struct FDamageData
// {
//     float  power = 0.0f;
//     class  ACharacter* Character;
//     class  AActor*     Causer;
//     struct FActionDamageEvent* Event;
// };



UCLASS()
class U06_BATTLE_API UCWeaponstructures : public UObject
{
	GENERATED_BODY()
	
};
