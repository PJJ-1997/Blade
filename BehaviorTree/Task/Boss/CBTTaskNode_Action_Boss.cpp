// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss/CBTTaskNode_Action_Boss.h"
#include "Global.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeaponstructures.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponAsset.h"
#include "Kismet/KismetMathLibrary.h"

UCBTTaskNode_Action_Boss::UCBTTaskNode_Action_Boss()
{
	this->NodeName = "Attack_Boss";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action_Boss::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	controller->StopMovement();
	
	weapon->SubAction_Pressed();

	return EBTNodeResult::InProgress;

}

EBTNodeResult::Type UCBTTaskNode_Action_Boss::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	
	if (weapon)
	{
		bool bBeginAction = weapon->GetDoAction()->GetBeginDoAction();
		if (!bBeginAction)
			weapon->GetDoAction()->Begin_DoAction();
		weapon->GetDoAction()->End_DoAction();
	}
	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Succeeded;
}
void UCBTTaskNode_Action_Boss::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	ACBoss_AI*     ai = Cast<ACBoss_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent*	state = CHelpers::GetComponent<UCStateComponent>(ai);

	bool bCheck = true;

	bCheck &= (state->IsIdleMode());
	//bCheck &= (weapon->GetDoAction()->GetInAction() == false);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
