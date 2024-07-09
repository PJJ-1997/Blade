// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
    CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

    {
        UMaterialInstanceConstant* matrial;
        CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&matrial, "MaterialInstanceConstant'/Game/Environment/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");
        Material = UMaterialInstanceDynamic::Create(matrial, this);
        Material->SetVectorParameterValue("Color", Color);
        Material->SetScalarParameterValue("Exp", Exponent);
    }

    Owner = Cast<ACharacter>(GetOwner());

    // 처음부터 잔상을 보여주면 안 되기 때문에 false
    Mesh->SetVisibility(false); 
    // Poseable에 복사
    Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
    Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
    Mesh->SetRelativeScale3D(Scale);

    // Material Assign
    for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)
    {
        Mesh->SetMaterial(i, Material);

    }

	// posable mesh timer를 이용하여 크기/위치 변경

    FTimerDelegate timerDelegate;

    timerDelegate.BindLambda([=]()
    {
            if (Mesh->IsVisible() == false)
                Mesh->ToggleVisibility();

            float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
            FVector location = Owner->GetActorLocation() - FVector(ScaleAmount.X, ScaleAmount.Y, height - ScaleAmount.Z);
            SetActorLocation(location);
            SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));
            
            Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());

    });
    // 0.2->0.2
    // WinAPIN인 timer는 Idle상태 timerEvent를 만듬

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);


}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}