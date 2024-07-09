// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_BasicAttack1.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Weapons/DoActions/CDoAction_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"


UCBTTaskNode_BasicAttack1::UCBTTaskNode_BasicAttack1()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UCBTTaskNode_BasicAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<ACBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Boss)
		return EBTNodeResult::Failed;

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target");
	AActor* OtherActor = Cast<AActor>(TargetObject);

	if (nullptr == OtherActor)
		return EBTNodeResult::Failed;

	// ���� �ε��� ����
	Boss->SetAttackIndex(0); // BasicAttack1�� �ش��ϴ� �ε��� ����
	Boss->BasicAttack1(OtherActor);

	// ���� ���� �α� ���
	UE_LOG(LogTemp, Warning, TEXT("Executing BasicAttack1 with AttackIndex: %d"), 0);

	IsAttacking = true;
	Boss->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
	//  ACBoss Ŭ�������� �� ���� �Լ�(BasicAttack1, BasicAttack2, BasicAttack3, BasicAttack4)���� SetAttackIndex ȣ��
	//	UCDoAction_Boss Ŭ������ OnAttachmentBeginOverlap �Լ����� ACBoss�� AttackIndex�� ������ ���
}

void UCBTTaskNode_BasicAttack1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
