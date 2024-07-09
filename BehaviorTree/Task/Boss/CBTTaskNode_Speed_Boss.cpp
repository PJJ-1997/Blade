// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_Speed_Boss.h"
#include "Global.h"
#include "BehaviorTree/CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "NavigationSystem.h"

UCBTTaskNode_Speed_Boss::UCBTTaskNode_Speed_Boss()
{
	this->NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed_Boss::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*				ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);

	movement->SetSpeed(Type);

	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Speed_Boss::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}