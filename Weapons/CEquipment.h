// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponstructures.h"
#include "CEquipment.generated.h"

////////////////////////////////////////////////////////////////
// Begin_equip,end_equip,unequip�� ������ ������
// Attacment�� ����
// sword�� ��ġ����
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


// ���α׷��Ӱ� �⺻���� ����� ����, �ٸ��μ�(����,��ȹ)���� BP_CAttachment_Sword
// ���������ϰԲ� ����

public:
    UFUNCTION(BlueprintNativeEvent)
        void Equip();                  // BP���� �̺�Ʈ�� ���̴� �Լ�
    void Equip_Implementation();       // C������ ���� 
    UFUNCTION(BlueprintNativeEvent)
        void Begin_Equip();
    void Begin_Equip_Implementation();
    UFUNCTION(BlueprintNativeEvent)
        void End_Equip();
    void End_Equip_Implementation();
    UFUNCTION(BlueprintNativeEvent)
        void UnEquip();
    void UnEquip_Implementation();


public: // ���������� Type�� ���� ����
    FEquipment_BeginEquip   OnEquipment_BeginEquip;
    FEquipment_EndEquip     OnEquipment_EndEquip;
    FEquipment_UnEquip      OnEquipment_UnEquip;

private:
    FEquipmentData Data;   // BeginPlay�� ����
    class UCMovementComponent* Movement;
    class UCStateComponent*    State;
private:
	UPROPERTY()
		class UCWeaponComponent*    Weapon;
protected:
    // Object�� ���� ����ȭ �Ǿ� �־�� �Ѵ�
    // UE���� ������ó�� ������� �ʴ� ��� �ڵ������� �����ϱ� ����
    UPROPERTY(BlueprintReadOnly)       
    class ACharacter*  OwnerCharacter; //Actor,ActorComponent ����ȭ�� �ʿ� ����

private:
    bool bBeginEquip = false;   // equip���۵Ǿ����� flag
    bool bEquipped   = false;   // �����Ǿ����� flag
public:
    FORCEINLINE       bool GetBeginEquip()  { return bBeginEquip; }
    FORCEINLINE const bool* GetEquipped()   { return &bEquipped; }
};
