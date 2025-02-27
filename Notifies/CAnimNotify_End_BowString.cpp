// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_End_BowString.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Bow.h"


FString UCAnimNotify_End_BowString::GetNotifyName_Implementation() const
{
    return "End_BowString";
}
void UCAnimNotify_End_BowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetDoAction());

    UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
    
    bow->End_BowString();

}

