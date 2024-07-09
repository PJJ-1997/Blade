 // Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UCSubAction_Sword::Pressed()
{
    CheckFalse(State->IsIdleMode());
    CheckTrue(State->IsSubActionMode());

    Super::Pressed();

    State->SetActionMode();
    State->OnSubActionMode();

    if (!!GhostTrailClass)
        GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);


    ActionData.DoAction(Owner);
}
// NotityState_SubAction
void UCSubAction_Sword::Begin_SubAction_Implementation()
{
    Super::Begin_SubAction_Implementation();

    // ���� DoAction�� Binding�� �� ����
    Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
    // Sword_SubAction Collision Check
    Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword::OnAttachmentBeginOverlap);

    bMoving = true;   // Tick���� ���̶�� �ൿ�� ó��
    Start = Owner->GetActorLocation();
    End = Start + Owner->GetActorForwardVector() * Distance;

	Owner->GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

    // Trace�� ���� Check
    float  radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
    float  height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    FRotator rotation = Owner->GetActorRotation();

    TArray<TEnumAsByte<EObjectTypeQuery>> objects;
    objects.Add(EObjectTypeQuery::ObjectTypeQuery3); // PAWN


    TArray<AActor*> ignores;
    ignores.Add(Owner);

    TArray<FHitResult> hitResults;


    UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height),
        rotation, objects, false, ignores, DrawDebug, hitResults, true);


    // Hit�� ���
    //  for (FHitResult hitResult : hitResults)  --> �ӵ����� X
    for (const FHitResult& hitResult : hitResults)
    {
        ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
        if (!!character)
        {
            character->GetCapsuleComponent()->SetCollisionProfileName("Sword_SubAction");
            Overlapped.Add(character);
        }

    }

    //  LineTrace�� �̿��Ͽ� ��Ʈ�Ȱ� Check

    FHitResult lineHitResult;
    UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1,
        false, ignores, DrawDebug, lineHitResult, true);


    if (lineHitResult.bBlockingHit)
    {
        FVector direction = (End - Start).GetSafeNormal2D();
        End = lineHitResult.Location - (direction * radius * 2);

        if (DrawDebug == EDrawDebugTrace::ForDuration)
        {
            DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Magenta, false, 2);
            DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, false, 5, 0, 3);
        }
            
    }

 //   if (DrawDebug == EDrawDebugTrace::ForDuration)
 //       DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true, 5, 0, 3);





}
// NotityState_SubAction
void UCSubAction_Sword::End_SubAction_Implementation()
{
    Super::End_SubAction_Implementation();
    // ���� SubAction�� Binding�� �� �����ϰ� DoAction
    Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
    Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

    bMoving = false;       // Hit�ȵǾ� �������� ó��
    State->SetIdleMode();
    State->OffSubActionMode();

    Movement->Move();
    Movement->DisableFixedCamera();


    if (!!GhostTrail)
        GhostTrail->Destroy();

	Hit.Empty();
	for (ACharacter* character : Overlapped)
	{
		character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");

		// ī�޶� �浹 ������ ���÷� ���� (ĸ�� ������Ʈ)
		character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

		// ī�޶� �浹 ������ ���÷� ���� (�޽� ������Ʈ)
		if (character->GetMesh())
		{
			character->GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		}
	}

	Overlapped.Empty();

	// ��ð� ���� �� �浹 ��Ȱ��ȭ
	Owner->GetCapsuleComponent()->SetCollisionProfileName("Pawn");

   // for (ACharacter* character : Overlapped)
   // {
   //     character->GetCapsuleComponent()->SetCollisionProfileName("");
   // }
   //
   // Overlapped.Empty();
}
void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
    Super::Tick_Implementation(InDeltaTime);

    CheckFalse(bMoving);

    FVector location = Owner->GetActorLocation();
    float   radius = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();


//    CLog::Print("Tick_Implementation ");

    if (location.Equals(End, radius))
    {
        bMoving = false;
        Start = End = Owner->GetActorLocation();
        return;
    }

    // Hit�Ȱ����� �̵�
    FVector direction = (End - Start).GetSafeNormal2D();
    Owner->AddActorWorldOffset(direction*Speed, true);

}
// DoAction-->��Class
void UCSubAction_Sword::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther)
{
    CheckNull(InOther);
    for (ACharacter* character : Hit)
        CheckTrue(character == InOther);

    Hit.AddUnique(InOther);

    HitData.SendDamage(Owner, InAttackCuaser, InOther);
}