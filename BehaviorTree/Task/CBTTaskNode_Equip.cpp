// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Characters/Enemy/CAIController.h"
#include "Weapons/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	this->NodeName  = "Equip";
	bNotifyTick		= true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	if (Type == weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Sword: weapon->SetSwordMode(); break;
	case EWeaponType::Warp: weapon->SetWarpMode(); break;
	case EWeaponType::Bow: weapon->SetBowMode(); break;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI*     ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	const bool *bEquipped = weapon->GetEquipment()->GetEquipped();

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
