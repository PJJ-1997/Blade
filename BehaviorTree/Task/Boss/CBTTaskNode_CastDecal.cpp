// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_CastDecal.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CBoss.h"

UCBTTaskNode_CastDecal::UCBTTaskNode_CastDecal()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UCBTTaskNode_CastDecal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Boss)
		return EBTNodeResult::Failed;

	Boss->DecalAttack();
	IsAttacking = true;

	Boss->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;

		});
	return EBTNodeResult::InProgress;


}

void UCBTTaskNode_CastDecal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
