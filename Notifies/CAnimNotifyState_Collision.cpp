// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Characters/CBoss.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
    return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    // ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetAttachment());

    weapon->GetAttachment()->OnCollisions();

}
void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    // ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetAttachment());

    weapon->GetAttachment()->OffCollisions();
}