// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CThornObject.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"


ACThornObject::ACThornObject()
{
    CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
    CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara",Root);


}

// Called when the game starts or when spawned
void ACThornObject::BeginPlay()
{
	Super::BeginPlay();
    // Callback�Լ�
    Niagara->SetNiagaraVariableObject("Collision", this);
    Niagara->OnSystemFinished.AddDynamic(this, &ACThornObject::OnSystemFinished);

    // DefaultMesh --> MeshRender�� assing�Ǿ� Meshũ�⸦ ������
    if (!!NiagaraMesh)
    {
        BoxExtent = NiagaraMesh->GetBoundingBox().GetExtent();
    }

	
}
// Collision : Position,Scale
void ACThornObject::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem * NiagaraSystem)
{
    CheckFalse(Data.Num() > 0); // �����Ͱ� ������ ����

    // HitCheckg�Ҷ� �������� �Ѵ� : 
    // && 

    static TArray<AActor*> ignores;
    ignores.AddUnique(GetOwner());

    static FHitResult hitResult;


    // Hit�Ǵ� ����� ������ for loop�ȿ� �ִ� ������
    // for loop���� �ְ� ���

    for (int32 i = (Data.Num() - 1); i >= 0; i--)
    {
        FVector position = Data[i].Position + GetActorLocation();
        FVector scale = Data[i].Velocity * BoxExtent;
        DrawDebugBox(GetWorld(), position, scale, FQuat(NiagaraMeshRotation), FColor::Red);
        CLog::Log(position);
        CLog::Log(scale);


        UKismetSystemLibrary::BoxTraceSingleByProfile(GetWorld(), position, position, scale, NiagaraMeshRotation,
            "Pawn", false, ignores, EDrawDebugTrace::ForOneFrame, hitResult, true);

        if (!hitResult.bBlockingHit)
            continue;
        float height = 0.0f;
        ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

        if (!!CollisionEffect)
        { 
            if (!!character)
                height = character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()*2.0f;

            // Effect��ġ�� �Ӹ� ==> ���鿡�� ���� 1.5��
            FTransform transform = CollisionEffectTransform;
            FVector location = FVector(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z - height);
            transform.AddToTranslation(location);
            CHelpers::PlayEffect(GetWorld(), CollisionEffect, transform);

        }
        if (!!character)
            HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);

    }



}

void ACThornObject::OnSystemFinished(UNiagaraComponent * PSyetem)
{
    Destroy();
}


