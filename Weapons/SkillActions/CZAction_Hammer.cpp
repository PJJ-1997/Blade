// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CZAction_Hammer.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

void UCZAction_Hammer::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsZKeyActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnZKeyActionMode();

	ActionData.DoAction(Owner);
}

void UCZAction_Hammer::Begin_ZAction_Implementation()
{
	Super::Begin_ZAction_Implementation();

	// ���� DoAction�� Binding�� �� ����
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	// Sword_SubAction Collision Check
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCZAction_Hammer::OnAttachmentBeginOverlap);

	bMoving = true;   // Tick���� ���̶�� �ൿ�� ó��

	// Trace�� ���� Check
	float  radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float  height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator rotation = Owner->GetActorRotation();

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3); // PAWN

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	TArray<FHitResult> hitResults;

	// Hit�� ���
	//  for (FHitResult hitResult : hitResults)  --> �ӵ����� X
	for (const FHitResult& hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
		if (!!character)
		{
			character->GetCapsuleComponent()->SetCollisionProfileName("Sword_ZAction");
			Overlapped.Add(character);
		}

	}

	CheckNull(ZAuraClass);


	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector ForwardVector = Owner->GetActorForwardVector();
	FVector  SocketLocation = Owner->GetMesh()->GetSocketLocation("Hand_Sword");
	FRotator SocketRotation = FRotator(Owner->GetActorForwardVector().Rotation().Pitch, Owner->GetActorForwardVector().Rotation().Yaw, Owner->GetMesh()->GetSocketRotation("Hand_Sword").Roll);

	FTransform Transform(SocketRotation, SocketLocation);
	Owner->GetWorld()->SpawnActor<ACZAura>(ZAuraClass, Transform, params);
}



void UCZAction_Hammer::End_ZAction_Implementation()
{
	Super::End_ZAction_Implementation();
	// ���� SubAction�� Binding�� �� �����ϰ� DoAction
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	bMoving = false;       // Hit�ȵǾ� �������� ó��
	State->SetIdleMode();
	State->OffZKeyActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	Hit.Empty();

	for (ACharacter* character : Overlapped)
	{
		character->GetCapsuleComponent()->SetCollisionProfileName("");
	}

	Overlapped.Empty();
}

void UCZAction_Hammer::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	CheckFalse(bMoving);

	FVector location = Owner->GetActorLocation();
	float   radius = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

}

void UCZAction_Hammer::OnAttachmentBeginOverlap(ACharacter * InAttacker, AActor * InAttackCuaser, ACharacter * InOther)
{
	CheckNull(InOther);
	for (ACharacter* character : Hit)
		CheckTrue(character == InOther);

	Hit.AddUnique(InOther);

	HitData.SendDamage(Owner, InAttackCuaser, InOther);
}
