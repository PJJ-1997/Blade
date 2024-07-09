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
        // BP에 Event로 Call, NotiEvent처리를 하기위해 우리가 함수를 강제적으로..
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

    if (OnEquipment_BeginEquip.IsBound())      //Attachement로  바인딩실행
        OnEquipment_BeginEquip.Broadcast();    //
}

void UCEquipment::End_Equip_Implementation()
{
    bBeginEquip = false;
    bEquipped = true;


    Movement->Move();      // CanMove를 true
    State->SetIdleMode();  // 장착이 끝난후 Idle모드로


    if (OnEquipment_EndEquip.IsBound())
        OnEquipment_EndEquip.Broadcast();

}

void UCEquipment::UnEquip_Implementation()
{
    bEquipped = false;
    if (OnEquipment_UnEquip.IsBound())
        OnEquipment_UnEquip.Broadcast();
}
