// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CXAction_Sword.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

void UCXAction_Sword::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsXKeyActionMode());

	// Trace를 통해 앞에 액터가 2m(200cm) 내에 있는지 확인
	FVector Start = Owner->GetActorLocation();
	FVector ForwardVector = Owner->GetActorForwardVector();
	FVector End = Start + (ForwardVector * 200.0f);  // 2 meters = 200 cm

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);

	FHitResult OutHit;
	bool bIsHit = Owner->GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionParams);


	if (bIsHit && OutHit.GetActor() != nullptr)
	{
		// 히트된 액터가 존재하면 X 키 발동
		Super::Pressed();

		State->SetActionMode();
		State->OnXKeyActionMode();

		ActionData.DoAction(Owner);
	}
}

void UCXAction_Sword::Begin_XAction_Implementation()
{
	Super::Begin_XAction_Implementation();

	Hit.Empty();

	// 기존 DoAction에 Binding된 것 삭제
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	// Sword_SubAction Collision Check
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCXAction_Sword::OnAttachmentBeginOverlap);

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

	FVector  ForwardVector = Owner->GetActorForwardVector();
	FVector  SocketLocation = Owner->GetMesh()->GetSocketLocation("Hand_Sword");
	FRotator SocketRotation = FRotator(Owner->GetActorForwardVector().Rotation().Pitch, Owner->GetActorForwardVector().Rotation().Yaw, Owner->GetMesh()->GetSocketRotation("Hand_Sword").Roll);

	FTransform Transform(SocketRotation, SocketLocation);
	Owner->GetWorld()->SpawnActor<ACZAura>(ZAuraClass, Transform, params);
}



void UCXAction_Sword::End_XAction_Implementation()
{
	Super::End_XAction_Implementation();

	// 기존 SubAction에 Binding된 것 삭제하고 DoAction
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	bMoving = false;       // Hit안되어 있을때도 처리
	State->SetIdleMode();
	State->OffXKeyActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	Hit.Empty();

	for (ACharacter* character : Overlapped)
	{
		character->GetCapsuleComponent()->SetCollisionProfileName("");
	}

	Overlapped.Empty();
}

void UCXAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	CheckFalse(bMoving);

	FVector location = Owner->GetActorLocation();
	float   radius = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

}

void UCXAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	// 중복 체크 대신 히트 몽타주가 항상 재생되도록 함
	Hit.AddUnique(InOther);
	HitData.SendDamage(Owner, InAttackCauser, InOther);

	UE_LOG(LogTemp, Warning, TEXT("Overlap with: %s"), *InOther->GetName());

	if (InOther->IsA(ACharacter::StaticClass()))
	{
		ACharacter* EnemyCharacter = Cast<ACharacter>(InOther);
		if (EnemyCharacter)
		{
			// 적 캐릭터를 공중으로 날리는 코드
			FVector LaunchDirection = EnemyCharacter->GetActorUpVector();
			LaunchDirection.Normalize();
			float LaunchForce = 1000.0f;
			EnemyCharacter->LaunchCharacter(LaunchDirection * LaunchForce, true, true);

			if (InAttacker)
			{
				InAttacker->LaunchCharacter(LaunchDirection * LaunchForce, true, true);
			}

			// 히트 몽타주 실행
			UAnimInstance* AnimInstance = InOther->GetMesh()->GetAnimInstance();
			if (AnimInstance && HitData.Montage)
			{
				UE_LOG(LogTemp, Warning, TEXT("Playing Hit Montage for: %s"), *InOther->GetName());
				AnimInstance->Montage_Play(HitData.Montage, HitData.PlayRate);
			}
		}
	}
}
