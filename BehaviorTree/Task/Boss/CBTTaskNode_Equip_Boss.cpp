// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_Equip_Boss.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Weapons/CEquipment.h"

UCBTTaskNode_Equip_Boss::UCBTTaskNode_Equip_Boss()
{
	this->NodeName = "Equip_Boss";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip_Boss::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	if (Type == weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Sword: weapon->SetSwordMode(); break;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Equip_Boss::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;
}

void UCBTTaskNode_Equip_Boss::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	const bool *bEquipped = weapon->GetEquipment()->GetEquipped();

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
