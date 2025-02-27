// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CDownAttack_Sword.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

void UCDownAttack_Sword::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsQKeyActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnQKeyActionMode();

	ActionData.DoAction(Owner);
}

void UCDownAttack_Sword::Begin_DownAttack_Implementation()
{
	Super::Begin_DownAttack_Implementation();

	// 기존 DoAction에 Binding된 것 삭제
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	// Sword_SubAction Collision Check
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCDownAttack_Sword::OnAttachmentBeginOverlap);

	bMoving = true;   // Tick에서 참이라면 행동을 처리

	// Trace를 통해 Check
	float  radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float  height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator rotation = Owner->GetActorRotation();

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3); // PAWN

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	TArray<FHitResult> hitResults;

	// Hit된 결과
	//  for (FHitResult hitResult : hitResults)  --> 속도문제 X
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



void UCDownAttack_Sword::End_DownAttack_Implementation()
{
	Super::End_DownAttack_Implementation();
	// 기존 SubAction에 Binding된 것 삭제하고 DoAction
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	bMoving = false;       // Hit안되어 있을때도 처리
	State->SetIdleMode();
	State->OffQKeyActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	Hit.Empty();

	for (ACharacter* character : Overlapped)
	{
		character->GetCapsuleComponent()->SetCollisionProfileName("");
	}

	Overlapped.Empty();
}

void UCDownAttack_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	CheckFalse(bMoving);

	FVector location = Owner->GetActorLocation();
	float   radius = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

}

void UCDownAttack_Sword::OnAttachmentBeginOverlap(ACharacter * InAttacker, AActor * InAttackCuaser, ACharacter * InOther)
{
	CheckNull(InOther);
	for (ACharacter* character : Hit)
		CheckTrue(character == InOther);

	Hit.AddUnique(InOther);

	HitData.SendDamage(Owner, InAttackCuaser, InOther);

	// 적을 바닥으로 코드 추가
	if (InOther->IsA(ACharacter::StaticClass()))
	{
		ACharacter* EnemyCharacter = Cast<ACharacter>(InOther);
		if (EnemyCharacter)
		{
			// 적을 바닥으로 코드 추가
			FVector DownwardDirection = FVector::DownVector;
			DownwardDirection.Normalize();
			float DownwardForce = 5000.0f;
			EnemyCharacter->LaunchCharacter(DownwardDirection * DownwardForce, true, true);
		}
	}
}

