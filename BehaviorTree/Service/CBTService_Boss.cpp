// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Boss::UCBTService_Boss()
{
	this->NodeName = "Melee";
	this->Interval = 0.1f;
	this->RandomDeviation = 0.0f;
}
void UCBTService_Boss::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (state->IsHitMode())
	{
		behavior->SetHittedMode();
	
		return;
	}
	
	if (state->IsActionMode())
	{
		// 공격 중이면 다른 행동을 하지 않도록 함
		return;
	}
	
	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
	{
		return;
	}
	
	float distance = ai->GetDistanceTo(target);
	
	if (distance < ActionRange)
	{
		behavior->SetActionMode();
		return;
	}
	
	behavior->SetApproachMode();

}
