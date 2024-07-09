// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTTaskNode_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
