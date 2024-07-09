// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_TeleportToPlayer.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "GameFramework/Actor.h"
#include "Characters/CPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTTaskNode_TeleportToPlayer::UCBTTaskNode_TeleportToPlayer()
{
	NodeName = "Teleport To Player";
}

EBTNodeResult::Type UCBTTaskNode_TeleportToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACAIController_Boss* AIController = Cast<ACAIController_Boss>(OwnerComp.GetAIOwner()))
	{
		if (APawn* AIPawn = AIController->GetPawn())
		{
			if (ACPlayer* PlayerCharacter = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				// 플레이어의 위치와 방향을 가져옵니다.
				FVector PlayerLocation = PlayerCharacter->GetActorLocation();
				FRotator PlayerRotation = PlayerCharacter->GetControlRotation();

				
				FVector Direction = PlayerRotation.Vector();
				FVector TeleportLocation = PlayerLocation + Direction * DistanceInFrontOfPlayer;

				
				AIPawn->SetActorLocation(TeleportLocation);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
