// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Weapons/CWeaponstructures.h"
#include "CBTTaskNode_Action_Boss.generated.h"

UCLASS()
class U06_BATTLE_API UCBTTaskNode_Action_Boss : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Action_Boss();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
