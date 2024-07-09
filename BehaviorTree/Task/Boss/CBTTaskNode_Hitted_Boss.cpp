// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_Hitted_Boss.h"
#include "Global.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Hitted_Boss::UCBTTaskNode_Hitted_Boss()
{
	this->NodeName = "Hitted_Boss";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Hitted_Boss::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());

	controller->StopMovement();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Hitted_Boss::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCStateComponent*	state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (!state->IsHitMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
}
