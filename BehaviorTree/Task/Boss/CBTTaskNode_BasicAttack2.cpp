// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_BasicAttack2.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CBoss.h"


UCBTTaskNode_BasicAttack2::UCBTTaskNode_BasicAttack2()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UCBTTaskNode_BasicAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	Boss->SetAttackIndex(1); // BasicAttack2에 해당하는 인덱스 설정
	Boss->BasicAttack2();

	// 공격 시작 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Executing BasicAttack2 with AttackIndex: %d"), 1);

	IsAttacking = true;
	Boss->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_BasicAttack2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
