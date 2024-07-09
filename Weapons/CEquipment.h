// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponstructures.h"
#include "CEquipment.generated.h"

////////////////////////////////////////////////////////////////
// Begin_equip,end_equip,unequip에 동작이 끝나면
// Attacment에 연결
// sword의 위치변경
///////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipment_BeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipment_EndEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipment_UnEquip);

UCLASS(Blueprintable)
class U06_BATTLE_API UCEquipment : public UObject
{
	GENERATED_BODY()
public:
    void  BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);


// 프로그래머가 기본값을 만들어 놓고, 다른부서(무기,기획)에서 BP_CAttachment_Sword
// 수정가능하게끔 설계

public:
    UFUNCTION(BlueprintNativeEvent)
        void Equip();                  // BP에서 이벤트로 보이는 함수
    void Equip_Implementation();       // C에서는 정의 
    UFUNCTION(BlueprintNativeEvent)
        void Begin_Equip();
    void Begin_Equip_Implementation();
    UFUNCTION(BlueprintNativeEvent)
        void End_Equip();
    void End_Equip_Implementation();
    UFUNCTION(BlueprintNativeEvent)
        void UnEquip();
    void UnEquip_Implementation();


public: // 델리게이터 Type과 변수 선언
    FEquipment_BeginEquip   OnEquipment_BeginEquip;
    FEquipment_EndEquip     OnEquipment_EndEquip;
    FEquipment_UnEquip      OnEquipment_UnEquip;

private:
    FEquipmentData Data;   // BeginPlay시 복사
    class UCMovementComponent* Movement;
    class UCStateComponent*    State;
private:
	UPROPERTY()
		class UCWeaponComponent*    Weapon;
protected:
    // Object인 경우는 직렬화 되어 있어야 한다
    // UE에서 개비지처리 사용하지 않는 경우 자동삭제를 방지하기 위해
    UPROPERTY(BlueprintReadOnly)       
    class ACharacter*  OwnerCharacter; //Actor,ActorComponent 직렬화할 필요 없음

private:
    bool bBeginEquip = false;   // equip시작되었는지 flag
    bool bEquipped   = false;   // 장착되었는지 flag
public:
    FORCEINLINE       bool GetBeginEquip()  { return bBeginEquip; }
    FORCEINLINE const bool* GetEquipped()   { return &bEquipped; }
};
