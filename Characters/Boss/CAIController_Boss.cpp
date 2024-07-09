// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Boss/CAIController_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CPlayer.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

const FName ACAIController_Boss::Distance(TEXT("Distance"));
const FName ACAIController_Boss::Target(TEXT("Target"));
const FName ACAIController_Boss::Direction(TEXT("Direction"));
const FName ACAIController_Boss::SelectAttackNumber(TEXT("SelectAttackNumber"));
const FName ACAIController_Boss::SelectAttackNumber2(TEXT("SelectAttackNumber2"));

ACAIController_Boss::ACAIController_Boss()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Enemies/Boss/BT_Boss.BT_Boss'"));
	if (BTObject.Succeeded())
	{
		BTBoss = BTObject.Object;
	}

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &this->Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	// Sight���� �� ������ �ʱ�ȭ
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
		Sight->SightRadius = 3000;
		Sight->LoseSightRadius = 3600;
		Sight->PeripheralVisionAngleDegrees = 120;  // �¿� --> 90, 
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
	// Look At Player
	Radius = 3000;
}
void ACAIController_Boss::BeginPlay()
{
	Super::BeginPlay();

	// ���ε�
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController_Boss::OnPerceptionUpdated);
}

void ACAIController_Boss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Boss = Cast<ACBoss_AI>(InPawn);

	SetGenericTeamId(Boss->GetTeamID()); // ���߿� �����Ǹ� Ȯ��

	//Enemy
	{
		// �츮�� ���� ������Ʈ�� Blackboard�� ����
		Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Boss);
		CheckNull(Behavior);
		Behavior->SetBlackboard(Blackboard);

		RunBehaviorTree(Boss->GetBehaviorTree());  // BP
	}

	if (ACPlayer* Player = Cast<ACPlayer>(InPawn))
	{
		// ��Ʈ�ѷ� ȸ�� ����
		Player->bUseControllerRotationYaw = false;

		if (UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement())
		{
			MovementComp->bOrientRotationToMovement = true;
			MovementComp->RotationRate = FRotator(0.0f, 300.0f, 0.0f); // ȸ�� �ӵ� ����
		}
	}
}
void ACAIController_Boss::OnUnPossess()
{
	Super::OnUnPossess();
}
void ACAIController_Boss::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;

	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	
	//  ������ Player Setting
	if (actors.Num() > 0)
	{
		CLog::Log(actors[0]->GetName());
		Blackboard->SetValueAsObject("Target", actors[0]);
		return;
	}
	Blackboard->SetValueAsObject("Target", nullptr);

	
}

void ACAIController_Boss::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
		UE_LOG(LogTemp, Warning, TEXT("AI Stopped"));
	}
}
void ACAIController_Boss::StartAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StartTree(*this->BTBoss, EBTExecutionMode::Looped);
	}
}
