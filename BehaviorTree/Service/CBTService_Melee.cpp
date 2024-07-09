// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/Service/CBTService_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/Enemy/CAIController.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	this->NodeName = "Melee";
	this->Interval = 0.1f;
	this->RandomDeviation = 0.0f;
}
void UCBTService_Melee::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state		    = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (state->IsHitMode())
	{
		behavior->SetHittedMode();

		return;
	}

	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
	{
		behavior->SetPatrolMode();
		return;
	}

	float distance = ai->GetDistanceTo(target);

	if (distance < ActionRange)
	{
		behavior->SetActionMode();

		return;
	}

	behavior->SetApproachMode();
	CLog::Log("ApprochMode");
}
