// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_Range.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/Enemy/CAIController.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Range::UCBTService_Range()
{
	this->NodeName = "Range";
	this->Interval = 0.1f;
	this->RandomDeviation = 0.0f;
}
void UCBTService_Range::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* contoller = Cast<ACAIController>(OwnerComp.GetOwner());  // BP Cast AI Controller
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(contoller->GetPawn());             // BP Cast Enemy

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	// Drawing
	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();
		start.Z -= 25;

		FVector end = start;
		end.Z += 50;

		// 0.1초마다 rendering이 일어남
		//DrawDebugCylinder(ai->GetWorld(), start, end, AvoidRange, 10, FColor::Red, true , this->Interval,0,0);
	}


	if (state->IsHitMode())
	{
		behavior->SetHittedMode();
		return;
	}

	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
	{
		contoller->ClearFocus(EAIFocusPriority::Gameplay);
		behavior->SetWaitMode();
		return;
	}
	contoller->SetFocus(target);


	float distance = ai->GetDistanceTo(target);
	if (distance < AvoidRange)
	{
		behavior->SetAvoidMode();
		return;

	}
	behavior->SetActionMode();

}
