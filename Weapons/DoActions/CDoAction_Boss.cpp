// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Boss.h"
#include "Global.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CBoss.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

void UCDoAction_Boss::DoAction()
{
	if (this->DoActionDatas.Num() < 1)
		return;

	if (bEnable)  // Combo Notity_State에서  bEnable을 True
	{
		bEnable = false;
		bExist = true;
		return;
	}

	CheckFalse(State->IsIdleMode());
	Super::DoAction();

	// 확률에따른 랜덤공격
	int32 RandomIndex = FMath::RandRange(0, 100);

	if (RandomIndex >= 50)
	{
		DoActionDatas[0].DoAction(OwnerCharacter);
		CLog::Log("DoActionDatas[0]");
		UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
	}
	else if (RandomIndex >= 20)
	{
		DoActionDatas[1].DoAction(OwnerCharacter);
		CLog::Log("DoActionDatas[1]");
		UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
	}
	else if (RandomIndex >= 0)
	{
		DoActionDatas[2].DoAction(OwnerCharacter);
		CLog::Log("DoActionDatas[2]");
		UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
	}

	// int32 RandomIndex = FMath::RandRange(0, DoActionDatas.Num() - 1);
	// DoActionDatas[RandomIndex].DoAction(OwnerCharacter);

}
void UCDoAction_Boss::Begin_DoAction()
{
	Super::Begin_DoAction();  // bBeginAction 처리
	CheckFalse(bExist);

	bExist = false;

	int32 RandomIndex = FMath::RandRange(0, 100);

	if (RandomIndex >= 50)
	{
		DoActionDatas[0].DoAction(OwnerCharacter);
		CLog::Log("DoActionDatas[0]");
		UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
	}
	else if (RandomIndex >= 20)
	{
		DoActionDatas[1].DoAction(OwnerCharacter);
		CLog::Log("DoActionDatas[1]");
		UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
	}
	else if (RandomIndex >= 0)
	{
		DoActionDatas[2].DoAction(OwnerCharacter);
		CLog::Log("DoActionDatas[2]");
		UE_LOG(LogTemp, Warning, TEXT("Selected Random Index: %d"), RandomIndex);
	}

}

void UCDoAction_Boss::End_DoAction()
{
	Super::End_DoAction(); // nInAction ,bBeginAction처리
	
	//Index = 0;
}

void UCDoAction_Boss::OnAttachmentBeginOverlap(ACharacter * InAttacker, AActor * InAttackerCauser, ACharacter * InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackerCauser, InOther);
	CheckNull(InOther);

	// 연속적으로 Hit되는 것을 방지
	for (ACharacter* hit : Hit)
		CheckTrue(hit == InOther);

	Hit.AddUnique(InOther);

	// ACBoss에서 AttackIndex 가져오기
	ACBoss* Boss = Cast<ACBoss>(InAttacker);
	int32 CurrentAttackIndex = Boss ? Boss->GetAttackIndex() : 0;
	CheckTrue(HitDatas.Num() - 1 < CurrentAttackIndex);
	HitDatas[CurrentAttackIndex].SendDamage(InAttacker, InAttackerCauser, InOther);

	// 데미지 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("AttackIndex: %d, Damage: %f"), CurrentAttackIndex, HitDatas[CurrentAttackIndex].Power);
	//Super::OnAttachmentBeginOverlap(InAttacker, InAttackerCauser, InOther);
	//CheckNull(InOther);
	//
	//// 기본형
	////InOther->TakeDamage(20.0f, FDamageEvent(), InAttacker->GetController(), InAttackerCauser);
	//
	//// 연속적으로 Hit되는것을 방지
	//for (ACharacter* hit : Hit)
	//	CheckTrue(hit == InOther);
	//
	//Hit.AddUnique(InOther);
	//CheckTrue(HitDatas.Num() - 1 < Index);
	//HitDatas[AttackIndex].SendDamage(InAttacker, InAttackerCauser, InOther);
	////HitDatas[Index].SendDamage(InAttacker, InAttackerCauser, InOther);
	//UE_LOG(LogTemp, Warning, TEXT("AttackIndex: %d, Damage: %f"), AttackIndex, HitDatas[AttackIndex].Power);
}
void UCDoAction_Boss::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	// Player쪽 Hit되면 바라보게 하기

	float angle = -2.0f;   // dot -1 ~ 1;
	ACharacter* candidate = nullptr;
	for (ACharacter* hit : Hit)
	{
		FVector direction = hit->GetActorLocation() - OwnerCharacter->GetTargetLocation();
		direction = direction.GetSafeNormal2D();   // NormaalLize BP에서 tolerence

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
		//       controller->SetControlRotation(target);
	}
	Hit.Empty();

}
