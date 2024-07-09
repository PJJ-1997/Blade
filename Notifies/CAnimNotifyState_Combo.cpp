// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
    return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    // ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetDoAction());


    UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
    CheckNull(combo);

    combo->EnableCombo();

}
void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetDoAction());


    UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
    CheckNull(combo);
    combo->DisableCombo();
}