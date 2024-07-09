// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/CBTService_LookAtPlayer.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "Characters/Boss/CBoss_AI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UCBTService_LookAtPlayer::UCBTService_LookAtPlayer()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UCBTService_LookAtPlayer::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();

	auto BossAI = Cast<ACAIController_Boss>(OwnerComp.GetAIOwner());
	DetectRadius = BossAI->Radius;

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());


	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ACPlayer* Player = Cast<ACPlayer>(OverlapResult.GetActor());

			if (Player && Player->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACAIController_Boss::Target, Player);


				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACAIController_Boss::Target, nullptr);
	}

}