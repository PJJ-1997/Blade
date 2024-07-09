// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Decorator/CBTDecorator_Distance.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTDecorator_Distance::UCBTDecorator_Distance()
{
	NodeName = TEXT("Distance");
}

bool UCBTDecorator_Distance::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<ACPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACAIController_Boss::Target));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) >= 1200.0f);
	return bResult;
}
