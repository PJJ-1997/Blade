// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Decorator/CBTDecorator_LowHealth.h"
#include "Characters/CBoss.h"
#include "Components/CStatusComponent.h"

UCBTDecorator_LowHealth::UCBTDecorator_LowHealth()
{
	NodeName = TEXT("Low Health");
}
bool UCBTDecorator_LowHealth::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = false;

	ACBoss* Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss)
	{
		if (Boss->Status->GetHealth() / Boss->Status->GetMaxHealth() <= 0.5f) // Changed to 50%
		{
			bResult = true;
		}
	}

	return bResult;
}
