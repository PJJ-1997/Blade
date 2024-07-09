// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCDoAction::UCDoAction()
{
}

void UCDoAction::BeginPlay(ACharacter * InOwner, ACAttachment * InAttachment, UCEquipment * InEquipment, const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
    OwnerCharacter = InOwner;
    Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
    State = CHelpers::GetComponent<UCStateComponent>(InOwner);
    World = OwnerCharacter->GetWorld();

    DoActionDatas = InDoActionData;
    HitDatas = InHitData;
}
void UCDoAction::DoAction()
{
    bInAction = true;
    State->SetActionMode();
}


void UCDoAction::Begin_DoAction()
{
    bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
    bInAction   = false;
    bBeginAction = false;

    State->SetIdleMode();
    Movement->Move();
    Movement->DisableFixedCamera();

}
