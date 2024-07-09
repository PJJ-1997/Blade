// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/CBTTaskNode_Speed.h"
#include "Global.h"
#include "BehaviorTree/CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Characters/Enemy/CAIController.h"
#include "NavigationSystem.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	this->NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*				ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);

	//CLog::Log(FString::Printf(L"%s,%p"), *ai->GetActorLabel(), this);
	movement->SetSpeed(Type);


	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Speed::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}
