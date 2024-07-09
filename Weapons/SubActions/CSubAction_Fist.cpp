// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Fist.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"


void UCSubAction_Fist::Pressed()
{

    CheckFalse(State->IsIdleMode());
    CheckTrue(State->IsSubActionMode());

    Super::Pressed();

    State->SetActionMode();
    State->OnSubActionMode();

    if(!!GhostTrailClass)
       GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);


    ActionData.DoAction(Owner);

}
void UCSubAction_Fist::Begin_SubAction_Implementation()
{
    Super::Begin_SubAction_Implementation();

    // 기존 DoAction에 Binding된 것 삭제
    Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
    Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");

    // FistSubAction Collision Check
    Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Fist::OnAttachmentBeginOverlap);
    Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSubAction_Fist::OnAttachmentEndCollision);



}
void UCSubAction_Fist::End_SubAction_Implementation()
{
    Super::End_SubAction_Implementation();

    Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
    Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");

    Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
    Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);


    // state
    State->SetIdleMode();
    State->OffSubActionMode();
    // movement
    Movement->Move();
    Movement->DisableFixedCamera();

    if (!!GhostTrail)
        GhostTrail->Destroy();
    HitIndex = 0;


}

void UCSubAction_Fist::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther)
{

    CheckNull(InOther);
    // Hit된 Character을 배열에 저장
    for (ACharacter* character : Hit)
    {
        if (InOther == character)
            return;

    }

    Hit.AddUnique(InOther);

    // HitIndex Check
    CheckTrue(HitIndex >= HitDatas.Num());
    HitDatas[HitIndex].SendDamage(Owner, InAttackCuaser, InOther);

}

void UCSubAction_Fist::OnAttachmentEndCollision()
{
    Hit.Empty();
    HitIndex++;  // 5가 Max
}