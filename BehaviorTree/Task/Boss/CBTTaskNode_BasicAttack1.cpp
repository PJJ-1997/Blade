// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_BasicAttack1.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Weapons/DoActions/CDoAction_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"


UCBTTaskNode_BasicAttack1::UCBTTaskNode_BasicAttack1()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UCBTTaskNode_BasicAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Boss)
		return EBTNodeResult::Failed;

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target");
	AActor* OtherActor = Cast<AActor>(TargetObject);

	if (nullptr == OtherActor)
		return EBTNodeResult::Failed;

	// 공격 인덱스 설정
	Boss->SetAttackIndex(0); // BasicAttack1에 해당하는 인덱스 설정
	Boss->BasicAttack1(OtherActor);

	// 공격 시작 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Executing BasicAttack1 with AttackIndex: %d"), 0);

	IsAttacking = true;
	Boss->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
	//  ACBoss 클래스에서 각 공격 함수(BasicAttack1, BasicAttack2, BasicAttack3, BasicAttack4)에서 SetAttackIndex 호출
	//	UCDoAction_Boss 클래스의 OnAttachmentBeginOverlap 함수에서 ACBoss의 AttackIndex를 가져와 사용
}

void UCBTTaskNode_BasicAttack1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
