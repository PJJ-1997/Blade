// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_CastSword.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CBoss.h"

UCBTTaskNode_CastSword::UCBTTaskNode_CastSword()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UCBTTaskNode_CastSword::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (nullptr == Boss)
		return EBTNodeResult::Failed;
	
	Boss->RangeAttack();
	IsAttacking = true;
	
	Boss->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	
		});
	return EBTNodeResult::InProgress;


}

void UCBTTaskNode_CastSword::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
