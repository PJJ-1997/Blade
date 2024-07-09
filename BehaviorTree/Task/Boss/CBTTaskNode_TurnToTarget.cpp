// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_TurnToTarget.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "Characters/Boss/CBoss_AI.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTTaskNode_TurnToTarget::UCBTTaskNode_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UCBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Boss)
		return EBTNodeResult::Failed;

	auto Target = Cast<ACPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACAIController_Boss::Target));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Boss->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Boss->SetActorRotation(FMath::RInterpTo(Boss->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
