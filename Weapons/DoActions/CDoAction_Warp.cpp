// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/DoActions/CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Components/CAIBehaviorComponent.h"
#include "Weapons/CAttachment.h"

UCDoAction_Warp::UCDoAction_Warp()
{

}
void UCDoAction_Warp::BeginPlay(class ACharacter*   InOwner, class ACAttachment* InAttachment, class UCEquipment*  InEquipment, const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
    Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionData, InHitData);
    Decal = CHelpers::GetComponent<UDecalComponent>(InAttachment);
    PlayerController = OwnerCharacter->GetController<APlayerController>();

}
void UCDoAction_Warp::Tick(float  InDeltaTime)
{
    Super::Tick(InDeltaTime);

    // DoAction중일때는 Decal Control 하지 않기
    CheckTrue(bInAction);


    FVector   location = FVector::ZeroVector;
    FRotator  rotation = FRotator::ZeroRotator;

    if (GetCusrorLocationAndRotation(location, rotation) == false)
    {
        Decal->SetVisibility(false);
        return;
    }

    // DoAction중일때는 Decal Control 하지 않기
    CheckTrue(bInAction);

    Decal->SetVisibility(true);
    Decal->SetWorldLocation(location);
    Decal->SetWorldRotation(rotation);

}
bool  UCDoAction_Warp::GetCusrorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
    CheckNullResult(PlayerController, false);

    FHitResult hitResult;

    PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
    CheckFalseResult(hitResult.bBlockingHit, false);

    OutLocation = hitResult.Location;
    OutRotation = hitResult.ImpactNormal.Rotation();
    return true;


}
void  UCDoAction_Warp::DoAction()
{
    CheckFalse(DoActionDatas.Num() > 0);
    CheckFalse(State->IsIdleMode());

    Super::DoAction();

    FRotator rotator = FRotator::ZeroRotator;

    if (GetCusrorLocationAndRotation(MoveToLocation, rotator))
    {
        // 지면아래로 잠기는 경우를 없애기 높이값 조정
        float height = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        MoveToLocation.Z = MoveToLocation.Z + height;

        float yaw = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), MoveToLocation).Yaw;
        OwnerCharacter->SetActorRotation(FRotator(0, yaw, 0));
    }

    DoActionDatas[0].DoAction(OwnerCharacter);

}
// Noti
void  UCDoAction_Warp::Begin_DoAction()
{
    Super::Begin_DoAction();

    if (!!PlayerController)
    {
        OwnerCharacter->SetActorLocation(MoveToLocation);
        MoveToLocation = FVector::ZeroVector;
    }

}