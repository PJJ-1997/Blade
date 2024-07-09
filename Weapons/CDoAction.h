// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponstructures.h"
#include "CDoAction.generated.h"

// CDoaction-->CDoAction_Sword
// class를 추상화 : virtual함수가 존재하면 추상클래스라고한다
//
//UCLASS(Abstract, NotBlueprintable)
UCLASS()

class U06_BATTLE_API UCDoAction : public UObject
{
	GENERATED_BODY()
	
public:
    UCDoAction();
public: // Actor,AtctorCompont Like하게
    virtual void BeginPlay
    (
        class ACharacter*   InOwner,
        class ACAttachment* InAttachment,
        class UCEquipment*  InEquipment,
        const TArray<FDoActionData>& InDoActionData, 
        const TArray<FHitData>& InHitData
    );
    virtual void Tick(float  InDeltaTime) {};  // 상속받는 곳에 override

public:
    virtual void DoAction();  
    virtual void Begin_DoAction();
    virtual void End_DoAction();

public:
    UFUNCTION()
        virtual void OnBeginEquip() {};
    UFUNCTION()
        virtual void OnUnEquip() {};

public:
	FORCEINLINE bool GetBeginDoAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }

protected:
    UPROPERTY()  // -> UObect 개비지콜렉션에서 삭제되지...(UProperty,TArray,61초)
        class ACharacter*          OwnerCharacter;
    UPROPERTY()
        class UCMovementComponent* Movement;
    UPROPERTY()
        class UCStateComponent*    State;
    UPROPERTY()
        class UWorld*              World;

public:  // Collision
    UFUNCTION()
        virtual void OnAttachmentBeginCollision() {};
    UFUNCTION()
        virtual void OnAttachmentEndCollision() {};
    UFUNCTION()
        virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker,AActor* InAttackerCauser, class ACharacter* InOther) {};
    UFUNCTION()
        virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {};


protected:
    TArray<FDoActionData>  DoActionDatas;
    TArray<FHitData>       HitDatas;

protected:
    bool bInAction    = false;
    bool bBeginAction = false;

};
