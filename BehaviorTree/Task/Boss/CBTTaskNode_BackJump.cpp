// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_BackJump.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "Characters/Boss/CBoss_AI.h"


UCBTTaskNode_BackJump::UCBTTaskNode_BackJump()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UCBTTaskNode_BackJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Boss)
		return EBTNodeResult::Failed;


	Boss->BackJump();
	IsAttacking = true;
	
	
	Boss->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	
		});
	return EBTNodeResult::InProgress;


}

void UCBTTaskNode_BackJump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
