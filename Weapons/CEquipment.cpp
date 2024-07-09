// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CEquipment.h"
#include <iostream>
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"


void  UCEquipment::BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData)
{
    OwnerCharacter = InOwner;
    Data = InData;
    Movement =   CHelpers::GetComponent<UCMovementComponent>(InOwner);
    State    =   CHelpers::GetComponent<UCStateComponent>(InOwner);

}

void UCEquipment::Equip_Implementation()
{
    State->SetEquipMode();
    if (Data.bCanMove == false)
        Movement->Stop();
    if (!!Data.Montage)
    {
        OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
    }
    else
    {
        // BP�� Event�� Call, NotiEventó���� �ϱ����� �츮�� �Լ��� ����������..
        Begin_Equip();
        End_Equip();

    }
	
	if (Data.bUseControlRotation)
	{
		Movement->EnableControlRotation();
	}
	
}

void UCEquipment::Begin_Equip_Implementation()
{
    bBeginEquip = true;

    if (OnEquipment_BeginEquip.IsBound())      //Attachement��  ���ε�����
        OnEquipment_BeginEquip.Broadcast();    //
}

void UCEquipment::End_Equip_Implementation()
{
    bBeginEquip = false;
    bEquipped = true;


    Movement->Move();      // CanMove�� true
    State->SetIdleMode();  // ������ ������ Idle����


    if (OnEquipment_EndEquip.IsBound())
        OnEquipment_EndEquip.Broadcast();

}

void UCEquipment::UnEquip_Implementation()
{
    bEquipped = false;
    if (OnEquipment_UnEquip.IsBound())
        OnEquipment_UnEquip.Broadcast();
}
