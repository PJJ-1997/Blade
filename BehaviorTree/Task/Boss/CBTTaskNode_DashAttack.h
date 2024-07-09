// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_DashAttack.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTTaskNode_DashAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_DashAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes) override;

private:
	bool IsAttacking = false;
};
