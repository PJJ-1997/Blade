// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_TeleportCenter.h"
#include "Characters/Boss/CAIController_Boss.h"

UCBTTaskNode_TeleportCenter::UCBTTaskNode_TeleportCenter()
{
	NodeName = TEXT("Teleport Boss to Center");
}

EBTNodeResult::Type UCBTTaskNode_TeleportCenter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController_Boss* AIController = Cast<ACAIController_Boss>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		if (AIPawn)
		{
			FVector CentralLocation = FVector(12020.0f, -12059.9f, -41.9f); // 맵 중앙 위치 (필요에 따라 수정)
			AIPawn->SetActorLocation(CentralLocation);

			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}