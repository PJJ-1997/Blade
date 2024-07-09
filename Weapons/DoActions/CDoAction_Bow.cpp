// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "AIController.h"
#include "Weapons/CEquipment.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Weapons/AddOns/CArrow.h"

UCDoAction_Bow::UCDoAction_Bow()
{

}
void  UCDoAction_Bow::BeginPlay(class ACharacter*   InOwner, class ACAttachment* InAttachment, class UCEquipment*  InEquipment, const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
    Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionData, InHitData);


    State = CHelpers::GetComponent<UCStateComponent>(InOwner);


    // Skeletal Mesh 

    SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
    PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

    ACAttachment_Bow *bow = Cast<ACAttachment_Bow>(InAttachment);
    if (!!bow)
        Bend = bow->GetBend();  // Ainmaion_Bow의 Bend의 주소를 저장

    // Local 좌표로 저장
    OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);
    bEquipped = InEquipment->GetEquipped();  // 주소

}
void  UCDoAction_Bow::OnBeginEquip()
{
    Super::OnBeginEquip();
    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CreateArrow();

}
void  UCDoAction_Bow::OnUnEquip()
{
    Super::OnUnEquip();

    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    *Bend = 0.0f; // 줄이 땡겨져 있을때는  원위치 ( ABP_Bow)
    PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);



    for (int32 i = Arrows.Num() - 1; i >= 0; i--)
    {
        // 손에 들고 있는 Arrow를 삭제
        if (!!Arrows[i]->GetAttachParentActor())
            Arrows[i]->Destroy();
    }

}
void  UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());
    CheckFalse(State->IsSubActionMode()); // IdleMode가 아니면 return
	
    Super::DoAction();
    DoActionDatas[0].DoAction(OwnerCharacter);

}
void  UCDoAction_Bow::Begin_DoAction()
{
    //CheckNull(OwnerCharacter);

    Super::Begin_DoAction();

    bAttachedString = false;
    *Bend = 0;

    PoseableMesh->SetBoneLocationByName("bow_string_mid",OriginLocation, EBoneSpaces::ComponentSpace);

    // 손에 있는 Arror를 가져오기
    ACArrow * arrow = GetAttacedArrow();
    CheckNull(arrow);

	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

    // Hit & Destory시의 Binding 처리

    arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
    arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay); // LifeSpan

    FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
    arrow->Shoot(forward);
}

void  UCDoAction_Bow::End_DoAction()
{
    Super::End_DoAction();
    CreateArrow();
}

void  UCDoAction_Bow::OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter)
{

    CheckFalse(HitDatas.Num() > 0);
    HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);

}
void  UCDoAction_Bow::OnArrowEndPlay(class ACArrow* InDestroyer)
{
    Arrows.Remove(InDestroyer);
}

void  UCDoAction_Bow::Tick(float  InDeltaTime)
{
	Super::Tick(InDeltaTime);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);

	CheckFalse(bCheck);

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
	FVector handLoction = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLoction, EBoneSpaces::WorldSpace);
}

void  UCDoAction_Bow::End_BowString()
{
    *Bend = 100.0f;
    bAttachedString = true;
}
void  UCDoAction_Bow::CreateArrow()
{

    FTransform transform;
    ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    CheckNull(arrow);

    arrow->AddIgnoreActor(OwnerCharacter);
    FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

    arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");
    Arrows.Add(arrow);

    UGameplayStatics::FinishSpawningActor(arrow, transform);


}

ACArrow * UCDoAction_Bow::GetAttacedArrow()
{
   // for (int32 i = Arrows.Num() - 1; i >= 0; i--)
   // {
   //     // 손에 들고 있는 Arrow를 삭제
   //     if (!!Arrows[i]->GetAttachParentActor())
   //         return Arrows[i];
   // }

    //	for (ACArrow* projectile : Arrows)
//	{
//		if (!projectile)
//			continue;
//		if (!!projectile->GetAttachParentActor())
//			return projectile;
//	}

	for (ACArrow* projectile : Arrows)
	{
		if (!projectile)
			continue;
		if (!!projectile->GetAttachParentActor())
			return projectile;
	}
    return nullptr;
}
