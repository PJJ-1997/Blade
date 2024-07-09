// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_Block.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CBoss.h"

UCBTTaskNode_Block::UCBTTaskNode_Block()
{
	this->NodeName = "Block";
}

EBTNodeResult::Type UCBTTaskNode_Block::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController_Boss* AIController = Cast<ACAIController_Boss>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		ACBoss* Boss = Cast<ACBoss>(AIController->GetPawn());
		if (Boss)
		{
			Boss->Block();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
