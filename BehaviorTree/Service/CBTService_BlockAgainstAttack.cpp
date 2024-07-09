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
		// �÷��̾��� ������ �����ϴ� ������ ���⿡ �߰��մϴ�.
		// ���÷� ������ �����ߴٰ� �����ϰ� BlackBoard�� ���� �����մϴ�.
		bool bIsPlayerAttacking = true; // ���� ���� �������� ��ü �ʿ�
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsPlayerAttacking", bIsPlayerAttacking);
	}
}
