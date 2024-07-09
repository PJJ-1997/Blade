// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Characters/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CStateComponent.h"
ACAttachment_Bow::ACAttachment_Bow()
{
    PrimaryActorTick.bCanEverTick = true;

    CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
    CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);



    USkeletalMesh *mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
    SkeletalMesh->SetSkeletalMesh(mesh);
    SkeletalMesh->SetCollisionProfileName("NoCollision");  // 활에 숨어있는 콜리젼을 꺼주기 위해

    // Animation추가

    {
        TSubclassOf<UCAnimInstance_Bow>  animInstance;
        CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "AnimBlueprint'/Game/Weapons/Bow/ABP_Bow.ABP_Bow_C'");
        SkeletalMesh->SetAnimInstanceClass(animInstance);
    }


}
void ACAttachment_Bow::BeginPlay()
{
    Super::BeginPlay();
    this->AttachTo("Holster_Bow");

    SkeletalMesh->SetVisibility(false);
    PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
    PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

}

void ACAttachment_Bow::Tick(float DelltaTime)
{
    Super::Tick(DelltaTime);

 //   return;
   CheckTrue(State->IsIdleMode());



   // PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
   // FVector handLoction = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
   // PoseableMesh->SetBoneLocationByName("bow_string_mid", handLoction, EBoneSpaces::WorldSpace);

}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
    Super::OnBeginEquip_Implementation();

    this->AttachTo("Hand_Bow_Left");

    // 카메라 조정
    APlayerController* contoller = OwnerCharacter->GetController<APlayerController>();
    if (!!contoller)
    {
        OriginViewPitchRange.X = contoller->PlayerCameraManager->ViewPitchMin;
        OriginViewPitchRange.Y = contoller->PlayerCameraManager->ViewPitchMax;

        contoller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
        contoller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;

    }



}

void ACAttachment_Bow::OnUnEquip_Implementation()
{
    Super::OnUnEquip_Implementation();

    this->AttachTo("Holster_Bow");

    // 카메라 조정
    APlayerController* contoller = OwnerCharacter->GetController<APlayerController>();
    if (!!contoller)
    {

        contoller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
        contoller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
    }

}

float * ACAttachment_Bow::GetBend()
{
    return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();
}
