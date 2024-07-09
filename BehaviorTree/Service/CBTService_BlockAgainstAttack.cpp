// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_BlockAgainstAttack.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTService_BlockAgainstAttack::UCBTService_BlockAgainstAttack()
{
	this->NodeName = "BlockAgainstAttack";
}

void UCBTService_BlockAgainstAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController_Boss* AIController = Cast<ACAIController_Boss>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		// 플레이어의 공격을 감지하는 로직을 여기에 추가합니다.
		// 예시로 공격을 감지했다고 가정하고 BlackBoard에 값을 설정합니다.
		bool bIsPlayerAttacking = true; // 실제 감지 로직으로 대체 필요
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsPlayerAttacking", bIsPlayerAttacking);
	}
}
