// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CPlayer.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &this->Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	// Sight���� �� ������ �ʱ�ȭ
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
		Sight->SightRadius = 1000;
		Sight->LoseSightRadius = 1200;
		Sight->PeripheralVisionAngleDegrees = 360;  // �¿� --> 90, 
		Sight->SetMaxAge(5.0f);
	}
	{
		// 1 : Player, 2: Enemy_AI, 255�߸� --> teamID
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
	}

	// Sight�� Setting�ϰ� Sight�� �ִ� ������ Perception Setting
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

}
void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	// ���ε�
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}
void ACAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
		UE_LOG(LogTemp, Warning, TEXT("AI Stopped"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTreeComponent is null in StopAI"));
	}
}
void   ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	
	SetGenericTeamId(Enemy->GetTeamID()); // ���߿� �����Ǹ� Ȯ��
	
	//Enemy
	{
	// �츮�� ���� ������Ʈ�� Blackboard�� ����
	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	CheckNull(Behavior);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());  // BP
	}

}
void   ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
void  ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> PerceivedActors;
	Perception->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);

	AActor* TargetActor = nullptr;
	float ClosestDistance = FLT_MAX;

	for (AActor* Actor : PerceivedActors)
	{
		if (ACPlayer* Player = Cast<ACPlayer>(Actor))
		{
			float Distance = FVector::Dist(Actor->GetActorLocation(), Enemy->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				TargetActor = Actor;
			}
		}
	}

	if (TargetActor)
	{
		CLog::Log(TargetActor->GetName());
		Blackboard->SetValueAsObject("Target", TargetActor);
	}
	else
	{
		Blackboard->SetValueAsObject("Target", nullptr);
	}
	//TArray<AActor*> actors;
	//
	//Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	//
	//
	////  ������ Player Setting
	//if (actors.Num() > 0)
	//{
	//	CLog::Log(actors[0]->GetName());
	//	Blackboard->SetValueAsObject("Target", actors[0]);
	//	return;
	//}
	//Blackboard->SetValueAsObject("Target", nullptr);
}