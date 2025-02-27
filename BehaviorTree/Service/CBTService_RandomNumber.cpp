// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_RandomNumber.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/Boss/CBoss_AI.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTService_RandomNumber::UCBTService_RandomNumber()
{
	this->NodeName = TEXT("CreateRandomNumber");
	Interval = 3.0f;

	bNotifyBecomeRelevant = true;
}

void UCBTService_RandomNumber::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	int x = 0;
	x = FMath::RandRange(1, 100);



	if (x <= 10)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber, 1);
	}
	else if (x <= 20)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber, 2);
	}
	else if (x <= 50)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber, 3);
	}
	else if (x <= 80)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber, 4);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber, 5);
	}

}
