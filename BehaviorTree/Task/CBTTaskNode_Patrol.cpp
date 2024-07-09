// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "BehaviorTree/CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Characters/Enemy/CAIController.h"
#include "NavigationSystem.h"	// 네비게이션볼륨
   

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	this->NodeName = "Patrol";
	bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);


	if (!!ai->GetPatrolPath())  // UI Setting Check
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint); // BB PatrolLocation

		//DrawDebug(ai->GetWorld(), moveToPoint);
		return EBTNodeResult::InProgress;
	}

	// 현재 월드에 배치되어 있는 볼륨가져오기
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);

	FVector location = ai->GetActorLocation();
	FNavLocation point(location);

	while (true)
	{
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point))
			break;
	}

	behavior->SetPatrolLocation(point.Location);
	//DrawDebug(ai->GetWorld(), point.Location);
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	FVector location = behavior->GetPatrolLocation();

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, true);

	switch (result)
	{

	case EPathFollowingRequestResult::Failed:
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		if (ai->GetPatrolPath())
			ai->GetPatrolPath()->UpdateIndex();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	break;
	}
}

void UCBTTaskNode_Patrol::DrawDebug(UWorld * InWorld, FVector InLocation)
{
	//if (bDebugMode)
	//	DrawDebugSphere(InWorld, InLocation, 10, 10, FColor::Green, true, 5);
}
