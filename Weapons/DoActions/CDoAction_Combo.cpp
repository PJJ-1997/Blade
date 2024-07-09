#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{
    if (this->DoActionDatas.Num() < 1)
        return;

    if (bEnable)  // Combo Notity_State����  bEnable�� True
    {
        bEnable = false;
        bExist = true;
        return;
    }

    CheckFalse(State->IsIdleMode());
    Super::DoAction();
    DoActionDatas[Index].DoAction(OwnerCharacter);

}
void UCDoAction_Combo::Begin_DoAction()
{
    Super::Begin_DoAction();  // bBeginActionó��
    CheckFalse(bExist);

    bExist = false;

    DoActionDatas[++Index].DoAction(OwnerCharacter);

}

void UCDoAction_Combo::End_DoAction()
{
    Super::End_DoAction(); // nInAction ,bBeginActionó��
    Index = 0;
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter * InAttacker, AActor * InAttackerCauser, ACharacter * InOther)
{
    Super::OnAttachmentBeginOverlap(InAttacker, InAttackerCauser, InOther);
    CheckNull(InOther);

    // �⺻��
    //InOther->TakeDamage(20.0f, FDamageEvent(), InAttacker->GetController(), InAttackerCauser);

    // ���������� Hit�Ǵ°��� ����
    for (ACharacter* hit : Hit)
        CheckTrue(hit == InOther);

    Hit.AddUnique(InOther);
    CheckTrue(HitDatas.Num() - 1 < Index);
    HitDatas[Index].SendDamage(InAttacker, InAttackerCauser, InOther);

}
void UCDoAction_Combo::OnAttachmentEndCollision()
{
    Super::OnAttachmentEndCollision();

    // Player�� Hit�Ǹ� �ٶ󺸰� �ϱ�

    float angle = -2.0f;   // dot -1 ~ 1;
    ACharacter* candidate = nullptr;
    for (ACharacter* hit : Hit)
    {
        FVector direction = hit->GetActorLocation() - OwnerCharacter->GetTargetLocation();
        direction = direction.GetSafeNormal2D();   // NormaalLize BP���� tolerence

        // FRoator -> FQuat ( Matrix)
        // m11 m12 m13 m14     ---> RightVector    11,12,13
        // m21 m22 m23 m24     ---> UpVector       21,22,23
        // m31 m32 m33 m34     ---> fowardVector   31,32,33
        // m41 m42 m43 m44
        FVector foward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

        float dot = FVector::DotProduct(direction, foward);
        if (dot >= angle)
        {
            angle = dot;
            candidate = hit;
        }
    }

    if (!!candidate)
    {
        FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
        FRotator target = FRotator(0, rotator.Yaw, 0);
        AController* controller = OwnerCharacter->GetController<AController>();
 //     controller->SetControlRotation(target);
    }
    Hit.Empty();
}
