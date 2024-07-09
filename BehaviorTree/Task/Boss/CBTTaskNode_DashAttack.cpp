// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_DashAttack.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CBoss.h"

UCBTTaskNode_DashAttack::UCBTTaskNode_DashAttack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UCBTTaskNode_DashAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Boss)
		return EBTNodeResult::Failed;


	Boss->DashAttack();
	IsAttacking = true;


	Boss->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		//LOG(Warning, TEXT("Attack ENd"));
		});
	return EBTNodeResult::InProgress;


}

void UCBTTaskNode_DashAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
