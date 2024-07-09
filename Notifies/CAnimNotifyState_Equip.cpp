// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_Equip.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CEquipment.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
    return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    // ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetEquipment());

    weapon->GetEquipment()->Begin_Equip();


}
void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    // ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetEquipment());

    weapon->GetEquipment()->End_Equip();
}