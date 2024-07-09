// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_Distance.h"
#include "Global.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"



UCBTService_Distance::UCBTService_Distance()
{
	NodeName = TEXT("Distance");
	Interval = 1.0f;
}

void UCBTService_Distance::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return;

	bool bResult;

	auto Target = Cast<ACPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACAIController_Boss::Target));

	if (nullptr == Target) return;

	float DistanceToTarget = Target->GetDistanceTo(ControllingPawn);
	bResult = (Target->GetDistanceTo(ControllingPawn) >= 1200.0f);

	UE_LOG(LogTemp, Warning, TEXT("Distance to Target: %f"), DistanceToTarget);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ACAIController_Boss::Distance, bResult);

}
