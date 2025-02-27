// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/SubActions/CSubAction_Hammer.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"
#include "Weapons/AddOns/CAura.h"

void UCSubAction_Hammer::Pressed()
{
    CheckFalse(State->IsIdleMode());
    CheckTrue(State->IsSubActionMode());

    Super::Pressed();

    State->SetActionMode();
    State->OnSubActionMode();

//	CLog::Print("SubAction");

    if (!!GhostTrailClass)
        GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);


    ActionData.DoAction(Owner);
}
// Auar�� Spawn
void UCSubAction_Hammer::Begin_SubAction_Implementation()
{
    Super::Begin_SubAction_Implementation();

    CheckNull(AuraClass);


    FActorSpawnParameters params;
    params.Owner = Owner;
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform transform;
	transform.SetLocation(Owner->GetActorLocation());
	transform.AddToTranslation(AuarLocation);
	transform.SetRotation(FQuat(Owner->GetActorRotation()));


    Owner->GetWorld()->SpawnActor<ACAura>(AuraClass, transform, params);  // Destory

}

void UCSubAction_Hammer::End_SubAction_Implementation()
{
    Super::End_SubAction_Implementation();

    // State
    State->SetIdleMode();
    State->OffSubActionMode();

    // Movement
    Movement->Move();
    Movement->DisableFixedCamera();

    // GhostTrail
    if (!!GhostTrail)
        GhostTrail->Destroy();
}
