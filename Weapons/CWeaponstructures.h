// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponstructures.generated.h"

// BP���� ���� Structure�� ����ȭ ���⿡ ����
// ��������� ���� ������ ����ü
// ����ü�� UE-C���� �����Ҷ��� ���� EditAnywhere

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
// DoAction�� ���õ� ������
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
    // ��ƼŬ , ���̾ư���
    UPROPERTY(EditAnywhere)
        class UFXSystemAsset* Effect; 
    UPROPERTY(EditAnywhere)
        FVector EffectLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere)
        FVector EffectScale   = FVector::OneVector;
public:
    void  DoAction(class ACharacter* InOwner);
};
// Hit�� ���õ� ������
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
