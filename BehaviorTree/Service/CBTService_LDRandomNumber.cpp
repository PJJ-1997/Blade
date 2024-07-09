// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_LDRandomNumber.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTService_LDRandomNumber::UCBTService_LDRandomNumber()
{
	NodeName = TEXT("LD_CreateRandomNumber");
	Interval = 1.0f;

	bNotifyBecomeRelevant = true;
}

void UCBTService_LDRandomNumber::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	int x = 0;
	x = FMath::RandRange(1, 100);

	if (x <= 40)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber2, 1);
	}
	else if (x <= 80)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber2, 2);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(ACAIController_Boss::SelectAttackNumber2, 3);
	}

}
